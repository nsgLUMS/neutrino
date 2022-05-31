# This program consumes config.json and run
# the epxierments according the configrations.
# Please checkout _config_readme_ for all the possible options.
# 
# The program can be run in the following manner
# Clean the exisiting binary of pktgen
# sudo make clean
# Create new binraies
# sudo make
# Run this program
# sudo python3 run_experiments.py [args]
# The program can take three possible arguments
#   --clean:    remove all the existing results in the results/ directory before running the experiments
#   --cta:      build cta before running the experiments
#   --pktgen:   build pktgen before running the experiments

from json import load
import subprocess
import os
from os.path import exists
from time import time
from shutil import move, rmtree
from time import sleep
from signal import SIGTERM, SIGINT, signal
from scp import SCPClient
from paramiko import SSHClient, AutoAddPolicy
from string import Template
from argparse import ArgumentParser
from functools import partial


timeout = time() + 60*2  # 2 mins
# ========= cta related constant parameters ===========
cta_log_files = ["CTA_console_logs.txt"]

# ========= Local cpf related constant parameters ===========
local_cpf_log_files = ["Local_CPF_console_logs.txt", "CPF_attach_logs.txt", 
                       "CPF_handover_logs.txt", "CPF_service_logs.txt"]

# ========= Remote cpf related constant parameters ===========
remote_cpf_log_files = ["Remote_CPF_console_logs.txt", "CPF_attach_logs.txt", 
                       "CPF_handover_logs.txt", "CPF_service_logs.txt"]

# ========= Common parameters =========================
core_args = Template("$scheme $replication $sync_time $pkt_logging $cpfs $local_cpfs $remote_cpfs $propagation_delay $georep_experiment")


# ========= Pktgen related constant parameters ===========
pktgen_log_files = ["pktgen_stats.txt", "pktgen_console_logs.txt", "PCT.txt"]
pktgen_conf = "config.json"
servers_credentials_conf = "servers_credentials.json"
pktgen_args = Template(
    "-d $duration -r $rate -u $workers -s $scheme -p $procedure -c $proc_count -g $georep_experiment")
pktgen_desc = Template(
    "Running exp $id of $scheme $proc in $mode with rep = $rep and logging = $log at rate = $rate")
cta_conn = None
local_cpf_conn = None
remote_cpf_conn = None

def arg_parser():
    parser = ArgumentParser(description="Fast-cta experiment scripts")
    parser.add_argument("--clean", type=bool, default=False,
                        nargs="?", help="clean logs", const=True)
    parser.add_argument("--cta", type=bool, default=False,
                        nargs="?", help="build cta before", const=True)
    parser.add_argument("--pktgen", type=bool, default=False,
                        nargs="?", help="build Pktgen before", const=True)
    return parser.parse_args()


def read_if_possible(chan):
    while chan.recv_ready():
        chan.recv(4096)


def run_cmd(cmd):
    subprocess.Popen([cmd], shell=True, stdout=None,
                     stderr=None, close_fds=True, preexec_fn=os.setsid)


def make_res_dir(kv, pktgen_credentials):

    pktgen_root = pktgen_credentials['root_dir']
    pktgen_res_dir = pktgen_root + "results/"

    results_dname = Template(pktgen_res_dir + "$mode-$scheme-$proc-$log-$rep/$rate/$itr/")

    log = "log" if kv["msg_logging"] else "nLog"
    rep = kv["curr_rep"]

    if kv["curr_rep"] == "time":
        rep = str(kv["sync_time"]) + "(ms)"
    elif kv["curr_rep"] == "nil":
        rep = "nR"

    mode = "b" if kv["bursty"] else "u"

    path = results_dname.substitute({"scheme":  kv["scheme"],
                                     "proc":    "-".join(kv["procedure"]),
                                     "itr":     kv["exp_id"],
                                     "rate":    kv["curr_rate"],
                                     "rep":     rep,
                                     "log":     log,
                                     "mode":    mode, })
    tmp_path = path + 'tmp/'
    if not os.path.exists(path):
        os.makedirs(path)
    if not os.path.exists(tmp_path):
        os.makedirs(tmp_path)
    # print(path)
    return path, tmp_path


def parse_core_args(conf):
    return core_args.substitute(conf)


def parse_pktgen_args(conf):
    return pktgen_args.substitute(conf)


def load_config(f_name):
    with open(f_name, 'r') as f:
        configs = load(f)
    return configs


def connect_entity(ip, username, self_key):
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.set_missing_host_key_policy(AutoAddPolicy())

    ssh.connect(ip, 22, username=username, key_filename=self_key)
    
    chan = ssh.get_transport().open_session()
    chan.invoke_shell()
    read_if_possible(chan)
    return ssh, chan

def run_core(conn, chan, root, logs_dir, entity, py_cmd, server_credentials):
    
    global timeout
    chan.sendall("cd {}\n".format(root))
    chan.sendall("mkdir -p {}\n".format(logs_dir))

    entity_id = 0
    if entity == 'local_cpf':
        entity_id = 1
    elif entity == 'remote_cpf':
        entity_id = 2
    else:
        entity_id = 3

    stdin, _, _ = conn.exec_command("cd {}; sudo python3 run.py {} {}\n".format(root, py_cmd, 
                                                                                          entity_id), get_pty=True)
    
    stdin.write(server_credentials['password'] + '\n')
    read_if_possible(chan)

    core_lck = "{}core.lck".format(root)
    while True:
        _, stdout, _ = conn.exec_command("ls {}".format(core_lck))
        status = stdout.readlines()
        if len(status) > 0 or time() > timeout:
            timeout = time() + 60*2
            break


def stop_core(entity, conn, root, server_credentials):
    stdin, stdout, stderr = conn.exec_command(
        "sudo pkill --signal SIGINT {}".format(entity), get_pty=True)

    stdin.write(server_credentials['password'] + '\n')

    stdin.flush()
    stdout.channel.recv_exit_status()
    stderr.channel.recv_exit_status()

    while True:
        _, stdout, _ = conn.exec_command("find {} -name core.lck".format(root), get_pty=True)
        status = stdout.readlines()
        if len(status) == 0:
            break


def build_pktgen():
    subprocess.call(["make clean && make"])


def run_pktgen(pktgen_cmd, pktgen_credentials):

    pktgen_root = pktgen_credentials['root_dir']
    pktgen_logs_dir = pktgen_root + "logs/"

    if not os.path.exists(pktgen_logs_dir):
        os.makedirs(pktgen_logs_dir)
    cmd = "{}build/pktgen -l 0-35 -n 4 --log-level=1 -- {} > {}pktgen_console_logs.txt 2>&1".format(
        pktgen_root, pktgen_cmd, pktgen_logs_dir)
    run_cmd(cmd)
    while not os.path.exists("{}pktgen.lck".format(pktgen_root)):
        pass

def prepare_cpfs_replicas_arguments(replicas):
    str_cmb = lambda x: ",".join(x)

    flat_list = []
    for i in range(len(replicas)):
        sp_list = []
        for sublist in list(replicas[i].items()):
            for item in sublist:
                if isinstance(item, list):
                    sp_list.append(",".join(item))
                else:        
                    sp_list.append(item)

        # print(sp_list)
        flat_list.append(sp_list)
    
    return "_".join(list(map(str_cmb, flat_list)))

def gen_configs_core(config):
    rep = config["curr_rep"]

    cta_conf = {
        "replication":
            0 if rep == "nil" else
            1 if rep == "msg" else
            2 if rep == "proc" else
            3 if rep == "time" else
            4 if rep == "batch" else
            0,
        "sync_time": config["sync_time"],
        "pkt_logging": 0 if config["msg_logging"] is False else 1,
        "cpfs": config["cpfs"],
        "scheme": 0 if config["scheme"] == "asn1" else 1,
        "propagation_delay": config["propagation_delay"],
        "local_cpfs": prepare_cpfs_replicas_arguments(config["local_cpfs"]),
        "remote_cpfs": prepare_cpfs_replicas_arguments(config["remote_cpfs"]),
        "georep_experiment": 0 if config["georep_experiment"] is False else 1
    }

    return cta_conf

def gen_configs_pktgen(config):
    
    pktgen_conf = {
        "duration":     config["duration"],
        "workers":      config["workers"],
        "scheme":       config["scheme"],
        "procedure":    config["curr_proc"],
        "rate":         config["curr_rate"] // config["workers"],
        "georep_experiment": 0 if config["georep_experiment"] is False else 1
    }
    return pktgen_conf


def stop_pktgen():
    run_cmd("pkill --signal SIGINT pktgen")
    while os.path.exists('pktgen.lck'):
        pass


def collect_logs(config, exp_id, servers_credentials):

    pktgen_credentials = servers_credentials['pktgen']
    core_credentials = servers_credentials['core_network']

    pktgen_logs_dir = pktgen_credentials['root_dir'] + "logs/"
    cta_logs_dir = core_credentials['cta']['root_dir'] + "logs/"
    local_cpf_logs_dir = core_credentials['local_cpf']['root_dir'] + "logs/"
    remote_cpf_logs_dir = core_credentials['remote_cpf']['root_dir'] + "logs/"


    # Move Pktgen logs to results dir
    config.update({"exp_id": exp_id})
    res_path, tmp_path = make_res_dir(config, pktgen_credentials)


    for pktgen_log in pktgen_log_files:
        src = os.path.join(pktgen_logs_dir + pktgen_log)
        if exists(src):
            dst = os.path.join(res_path + pktgen_log)
            move(src, dst)

    # Fetch cta logs to results dir
    with SCPClient(cta_conn.get_transport()) as scp:
        for cta_log in cta_log_files:
            scp.get(cta_logs_dir + cta_log, local_path=res_path)

    with SCPClient(local_cpf_conn.get_transport()) as scp:
        for cpf_log in local_cpf_log_files:
            scp.get(local_cpf_logs_dir + cpf_log, local_path=res_path)

    with SCPClient(remote_cpf_conn.get_transport()) as scp:
        for cpf_log in remote_cpf_log_files:
            scp.get(remote_cpf_logs_dir + cpf_log, local_path=tmp_path)


    for cpf_log in remote_cpf_log_files:
        if cpf_log == "Remote_cpf_console_logs.txt":
            os.replace(tmp_path + cpf_log, res_path + cpf_log)
        else:
            remote_file = open(tmp_path + cpf_log, 'r')
            local_file = open(res_path + cpf_log, 'a')

            local_file.write(remote_file.read())

            remote_file.close()
            local_file.close()

    rmtree(tmp_path)

    # file = ""
    # print(config["procedure"][1])
    # if (config["procedure"][1] == "handover"):
    #     file = 'cpf_handover_logs.txt'
    # else:
    #     file = 'cpf_service_logs.txt'
        
    # f = open(res_path + file, "a")
    # f1 = open(res_path + 'Remote_cpf_logs.txt', "r")
    # f.write(f1.read())
    # f.close()
    # os.remove(res_path + 'Remote_cpf_logs.txt')


def print_desc(exp_conf, exp_id):
    desc = pktgen_desc.substitute({
        "id": exp_id,
        "scheme": exp_conf["scheme"],
        "proc": exp_conf["curr_proc"],
        "rep": exp_conf["curr_rep"],
        "log": str(exp_conf["msg_logging"]),
        "rate": exp_conf["curr_rate"],
        "mode": "bursty" if exp_conf["bursty"] else
                "uniform"
    })
    print(desc)


def run_core_entities(exp_conf, servers_credentials):
    global cta_conn
    global local_cpf_conn
    global remote_cpf_conn

    core_conf = gen_configs_core(exp_conf)

    core_credentials = servers_credentials["core_network"]
    pktgen_credentials = servers_credentials["pktgen"]

    cta_root = core_credentials['cta']['root_dir']
    local_cpf_root = core_credentials['local_cpf']['root_dir']
    remote_cpf_root = core_credentials['remote_cpf']['root_dir']

    cta_logs_dir = cta_root + "logs/"
    local_cpf_logs_dir = local_cpf_root + "logs/"
    remote_cpf_logs_dir = remote_cpf_root + "logs/"

    local_cpf_conn, local_cpf_chan = connect_entity(core_credentials['local_cpf']['ip'], 
                                                    core_credentials['local_cpf']['username'],
                                                    pktgen_credentials['public_key_path'])
    run_core(local_cpf_conn, local_cpf_chan, local_cpf_root, 
             local_cpf_logs_dir, 'local_cpf', parse_core_args(core_conf),
             core_credentials['local_cpf'])
    print("Started Local CPF(s)")

    remote_cpf_conn, remote_cpf_chan = connect_entity(core_credentials['remote_cpf']['ip'], 
                                                      core_credentials['remote_cpf']['username'],
                                                      pktgen_credentials['public_key_path'])

    run_core(remote_cpf_conn, remote_cpf_chan, remote_cpf_root, 
             remote_cpf_logs_dir, 'remote_cpf', parse_core_args(core_conf),
             core_credentials['remote_cpf'])
    print("Started Remote CPF(s)")


    cta_conn, cta_chan = connect_entity(core_credentials['cta']['ip'], 
                                        core_credentials['cta']['username'],
                                        pktgen_credentials['public_key_path'])
    run_core(cta_conn, cta_chan, cta_root, cta_logs_dir, 'cta', parse_core_args(core_conf),
             core_credentials['cta'])
    print("Started CTA")

def stop_core_entities(exp_conf, exp_id, servers_credentials):
    global cta_conn
    global local_cpf_conn
    global remote_cpf_conn

    core_credentials = servers_credentials["core_network"]
    pktgen_credentials = servers_credentials["pktgen"]

    cta_root = core_credentials['cta']['root_dir']
    local_cpf_root = core_credentials['local_cpf']['root_dir']
    remote_cpf_root = core_credentials['remote_cpf']['root_dir']

    print("Stopping CTA")
    stop_core('cta', cta_conn, cta_root, core_credentials['cta'])
    # sleep(3)
    print("Stopping Local CPF(s)")
    stop_core('cpf', local_cpf_conn, local_cpf_root, core_credentials['local_cpf'])
    sleep(3)
    print("Stopping Remote CPF(s)")
    stop_core('cpf', remote_cpf_conn, remote_cpf_root, core_credentials['remote_cpf'])
    sleep(3)
    print("Collecting logs")

    collect_logs(exp_conf, exp_id, servers_credentials)

    cta_conn.close()
    local_cpf_conn.close()
    remote_cpf_conn.close()

    print("Experiment done ... !!!")

def run_exp(exp_id, exp_conf, rate, procedure, proc_count, servers_credentials):
 
    print("============= Starting New Experiment =============")
    exp_conf["curr_rate"] = rate
    exp_conf["curr_proc"] = procedure

    print_desc(exp_conf, exp_id)
    pktgen_conf = gen_configs_pktgen(exp_conf)

    pktgen_conf["proc_count"] = proc_count
    pktgen_credentials = servers_credentials['pktgen']
    run_pktgen(parse_pktgen_args(pktgen_conf), pktgen_credentials)
    print("Started pktgen")

    duration = exp_conf["duration"]
    sleep(duration)

    print("Stopping pktgen")
    stop_pktgen()

    sleep(8)
    if exp_conf["bursty"]:
        exp_conf["curr_rate"] = proc_count


def run_burst_experiment(confs, exp_id, rate, procedure, servers_credentials):
    for proc_count in confs["proc_count"]:
        run_exp(exp_id, confs, rate, procedure, proc_count, servers_credentials)


def run_uniform_experiment(confs, exp_id, rate, procedure, servers_credentials):
    run_exp(exp_id, confs, rate, procedure, 0, servers_credentials)


def run_from_configs(confs, servers_credentials):
    repeat = confs["repeat"]

    # For each experiment
    for exp in confs["experiments"]:
        # For every repeat
        for exp_id in range(1, repeat + 1):
            # For every replication type
            for rep in exp["replication"]:
                exp.update({"curr_rep": rep})
                run_core_entities(exp, servers_credentials)
                # For each rate value
                for rate in exp["rate"]:
                    # For each iteration
                    for proc in exp["procedure"]:
                        if exp["bursty"]:
                            run_burst_experiment(exp, exp_id, rate, proc, servers_credentials)
                        else:
                            run_uniform_experiment(exp, exp_id, rate, proc, servers_credentials)
                stop_core_entities(exp, exp_id, servers_credentials)


def sigint_handler(*args):
    global cta_conn
    global local_cpf_conn
    global remote_cpf_conn

    core_credentials = args[0]["core_network"]
    cta_root = core_credentials['cta']['root_dir']
    
    stop_pktgen()
    stop_core('cta', cta_conn, cta_root, core_credentials['cta'])
    stop_core('cpf', local_cpf_conn, core_credentials['local_cpf'])
    stop_core('cpf', remote_cpf_conn, core_credentials['remote_cpf'])
    exit(0)


def clean_logs(servers_credentials):
    pktgen_root = servers_credentials['pktgen']['root_dir']
    pktgen_res_dir = pktgen_root + "results/"

    if os.path.exists(pktgen_res_dir):
        print("Removing {} because of --clean flag".format(pktgen_res_dir))
        rmtree(pktgen_res_dir)


def main():
    args = arg_parser()

    confs = load_config(pktgen_conf)
    servers_credentials = load_config(servers_credentials_conf)

    if args.clean:
        clean_logs(servers_credentials)

    signal(SIGINT, partial(sigint_handler, servers_credentials))

    run_from_configs(confs, servers_credentials)


if __name__ == "__main__":
    main()
