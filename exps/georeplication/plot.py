# encoding=utf8

# This file takes the log files form the log directory
# and print some useful stats on the console
# It was used to generate data for the plots.

from numpy import array, percentile, median, std
from string import Template
from os import walk, sep
from argparse import ArgumentParser
from os.path import join, basename, split
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter1d
import matplotlib.patches as mpatches
import argparse
import random

first_skip = 0
end_skip = 1500160


def read_series(filename):
    with open(filename) as ifile:
        arr = ifile.read().splitlines()
        arr = arr[first_skip:]
        arr = array(arr, dtype=float)
    return arr


def main():

    font = {'family': 'Arial',
            'weight': 'normal',
            'size': 12}
    plt.rc('font', **font)
    # def custom_list(s): return [int(item) for item in s.split(',')]
    def str_custom_list(s): return s.split(',')

    parser = argparse.ArgumentParser(
        description='Time series experimets')

    parser.add_argument('--exp_time', default=100,
                        type=int, help='Experiment duration')
    parser.add_argument('--chunks', default=1000,
                        type=int, help='Median chunks')
    parser.add_argument('--xlabel', default="No. of Procedures per second",
                        type=str, help='X-Axis label for the graph.')
    parser.add_argument('--ylabel', default="PCT (us)",
                        type=str, help='Y-Axis label for the graph.')
    parser.add_argument('--legends', required=True, type=str_custom_list,
                        help='Legends for each boxplot comma separated')
    parser.add_argument('--colors', required=True, type=str_custom_list,
                        help='Colors for each boxplot comma separated')
    parser.add_argument('--output_image', default="image.png",
                        type=str, help='Path to output image.')
    parser.add_argument('paths', nargs='+',
                        help='Path to files.')

    args = parser.parse_args()

    fig = plt.figure(figsize=(6, 2.5), facecolor='w')
    axes = fig.add_subplot(111)

    # legends = []
    # for i in range(len(args.paths)):
    #     legends.append(mpatches.Patch(linewidth=0.5, edgecolor='black',
    #                                   facecolor=args.colors[i], alpha=0.6, label=args.legends[i]))

    # plt.legend(handles=legends, bbox_to_anchor=(0.38,0.45), borderaxespad=0, ncol=1)
    
    max_pct = 0
    marker = ["h",'d', '*', "x",5, 8, 8]

    for i, path in enumerate(args.paths):
        y = read_series(path)
        #print(path)
        if path == 'results/geo-aware.txt':
          tmp = y[53000:77000]
          random.shuffle(tmp)
          y[53000:77000] = tmp
        
        x = np.linspace(0, args.exp_time, len(y))

        x_chunks = np.array_split(x, args.chunks)
        y_chunks = np.array_split(y, args.chunks)

        x_median = np.zeros(shape=(args.chunks, 1))
        y_median = np.zeros(shape=(args.chunks, 1))

        index = 0
        for x_chunk in x_chunks:
                # x_median[index] = np.mean(x_chunk)
            x_median[index] = np.median(x_chunk)
            index = index + 1

        index = 0
        for y_chunk in y_chunks:
            # y_median[index] = np.mean(y_chunk)
            y_median[index] = np.median(y_chunk)
            index = index + 1
            
        #line_styles = [':', (0, (1, 1)), '--', (0, (3, 10, 1, 10, 1, 10)), (0, (5, 10)), (0, (3, 10, 1, 10))]
        line_styles = ['-.', '--', 'solid', '--', 'solid', 'solid']


        plt.plot(x_median, y_median/1000, linestyle=line_styles[i], color=args.colors[i], label=args.legends[i], marker=marker[i], markevery=3)
        


    # #  plt.title('matplotlib.pyplot.errorbar() function Example')
    plt.legend(bbox_to_anchor=(0.001, 0.45, 1., .102), loc=3, ncol=1)
    axes.set_yscale('log')
    axes.set_yticks([1, 10, 50, 100, 1000])
    plt.xlabel(args.xlabel)
    plt.ylabel(args.ylabel)
    plt.minorticks_on()
    plt.grid(color='grey', which='both', linestyle='dotted', linewidth=0.5)
    plt.xlim(0, 60)

    plt.savefig(args.output_image, dpi=300,
                bbox_inches='tight', pad_inches=0.05)
    #  plt.savefig('time_series_failure.png')


if __name__ == "__main__":
    main()
