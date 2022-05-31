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
georep_experiment=$georep_experiment;
sync_time=$sync_time;
pkt_logging=$pkt_logging;
number_of_cpf=$cpfs;
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
    georep_experiment = sys.argv[9]

    str_split = lambda x: x.split(',')
    
    config = Template(template)

    local_cpf_replicas = cpf_replicas_config(local_cpfs)
    remote_cpf_replicas = cpf_replicas_config(remote_cpfs)

    # print(cpfs_actions)
    res = config.substitute({'serializer': serializer, 'replication': replication,
                             'sync_time': sync_time, 'pkt_logging': pkt_logging,
                             'cpfs': cpfs, 'local_cpfs': local_cpf_replicas, 
                             'remote_cpfs': remote_cpf_replicas,
                             'georep_experiment': georep_experiment})

    config_file = open("cta_config.cfg", "w")
    config_file.write(res)
    config_file.close()

    proc = run_async(
        "sudo ./build/cta -l 0-13 -n 16 --log-level=1 -- -q 8 -p 0,1,2,3 > logs/CTA_console_logs.txt 2>&1")
    proc.wait()

    if os.path.exists("core.lck"):
        os.remove("core.lck")


if __name__ == "__main__":
    run()
