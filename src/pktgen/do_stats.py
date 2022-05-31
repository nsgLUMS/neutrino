# This file takes the log files form the log directory
# and print some useful stats on the console
# It was used to generate data for the plots.

from numpy import array, percentile, median, std
from string import Template
from os import walk, sep
from argparse import ArgumentParser
from os.path import join, basename, split

root_dir = "results"
pct_files = ["CPF_attach_logs.txt", "CPF_handover_logs.txt", "CPF_service_logs.txt"]
skip = 0


header = ["Exp", "90th Percentile", "99th Percentile", "25th Percentile",
          "75th Percentile", "Max", "Min", "Median", "Avg", "Std Dev"]

csv_print = Template(
    "$rate $min $perc_25 $median $perc_75 $perc_99 $max $avg $std_dev $count")

# csv_print = Template(
#     "$rate, 50=> $median, 99=>$perc_99, $min $perc_25  $perc_75 $max $avg $std_dev $count")


def read_series(filename):
    with open(filename) as ifile:
        arr = ifile.read().splitlines()
        arr = arr[skip:]
        arr = array(arr, dtype=float)
    return arr


def merge_series(parent_dir):
    merge_series = []
    for dirpath, dirname, _ in walk(parent_dir):
        if not dirname:
            for file in pct_files:
                series = read_series(join(dirpath, file))
                merge_series.append(series)
    return merge_series


def cal_stats(series):
    return {
        "count": len(series),
        "perc_90": percentile(series, 90),
        "perc_99": percentile(series, 99),
        "perc_25": percentile(series, 25),
        "perc_75": percentile(series, 75),
        "max": max(series),
        "min": min(series),
        "median":  median(series),
        "avg": sum(series)/len(series),
        "std_dev": std(series),
    }


def parse_tx():
    pass


def parse_rx():
    pass


def get_exp_name(path):
    return basename(path)


def print_stats(stats, path):
    # print("========== {} ==========".format(fname))
    # print(", ".join(header))
    # print(stats)
    stats.update({"rate": get_exp_name(path)})

    p_stats = csv_print.substitute(stats)
    print(p_stats)


def main():
    # args = arg_parser()
    # global pct_file
    # pct_file = args.f
    # print(pct_file)

    for dirpath, _, _ in walk(root_dir):
        if dirpath.count(sep) == 2:
            series = merge_series(dirpath)
            for i in range(len(pct_files)):
                if series[i].size != 0:
                    stats = cal_stats(series[i])
                    print_stats(stats, dirpath)


if __name__ == "__main__":
    main()
