# This script is used the pktgen to run the experiments.

import sys
import subprocess
import os
from time import sleep
from string import Template

replicas_string = '''
    {
        cpf = $cpf,
        replicas = $replicas
    }'''

template = """\
serializer=$serializer;
replication=$replication;
sync_time=$sync_time;
pkt_logging=$pkt_logging;
number_of_cpf=$cpfs;
propagation_delay=$propagation_delay;
entity_id=$entity_id;
local_cpfs=($local_cpfs
);
remote_cpfs=($remote_cpfs
);\
"""

def run_async(cmd):
    return subprocess.Popen([cmd], shell=True, stdout=None, stderr=None, close_fds=True, preexec_fn=os.setsid)

def cpf_replicas_config(cpfs_replicas):
    str_split = lambda x: x.split(',')

    replicas_template = Template(replicas_string)
    replicas_decoded = list(map(str_split, cpfs_replicas.split('_')))

    cpfs_replicas = ""
    for i in range(len(replicas_decoded)):
        res = replicas_template.substitute({'cpf': replicas_decoded[i][0], 
                                            'replicas': list(map(int, replicas_decoded[i][1:]))})
        cpfs_replicas += res 
        if i != len(replicas_decoded) - 1:
            cpfs_replicas += ","
    
    return cpfs_replicas

def run():
    serializer = sys.argv[1]
    replication = sys.argv[2]
    sync_time = sys.argv[3]
    pkt_logging = sys.argv[4]
    cpfs = sys.argv[5]
    local_cpfs = sys.argv[6]
    remote_cpfs = sys.argv[7]
    propagation_delay = sys.argv[8]
    georep_experiment = sys.argv[9]
    entity_id = sys.argv[10]

    str_split = lambda x: x.split(',')
    
    config = Template(template)

    local_cpf_replicas = cpf_replicas_config(local_cpfs)
    remote_cpf_replicas = cpf_replicas_config(remote_cpfs)

    res = config.substitute({'serializer': serializer, 'replication': replication,
                             'sync_time': sync_time, 'pkt_logging': pkt_logging,
                             'cpfs': cpfs, 'local_cpfs': local_cpf_replicas, 
                             'remote_cpfs': remote_cpf_replicas,
                             'propagation_delay': propagation_delay, 'entity_id': entity_id})

    config_file = open("cpf_config.cfg", "w")
    config_file.write(res)
    config_file.close()

    file_prefix = ""
    if entity_id == '1':
        file_prefix = "Local"
    elif entity_id == '2':
        file_prefix = "Remote"

    proc = run_async("sudo ./build/cpf -l 0-27 -n 4 --log-level=1 -- -q 8 -p 0,1,2,3 > logs/{}_CPF_console_logs.txt 2>&1".format(file_prefix))
    proc.wait()

    if os.path.exists("core.lck"):
        os.remove("core.lck")


if __name__ == "__main__":
    run()
