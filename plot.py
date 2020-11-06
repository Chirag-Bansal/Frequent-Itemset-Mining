import os
import sys
import argparse
from timeit import timeit
from matplotlib import pyplot as plt

def plot(apTimes, fpTimes,supports):
    plt.plot(supports, apTimes, label='A Priori')
    plt.plot(supports, fpTimes, label='FP-Tree')
    plt.xlabel('Support threshold in Percentage')
    plt.ylabel('Time taken(s)')
    plt.title('Execution time vs support for Apriori and FP-tree')
    plt.legend(loc='upper right')
    plt.show()
    plt.savefig('graph.pdf')

def plotLog(apTimes, fpTimes,supports):
    plt.yscale('log')
    plt.plot(supports, apTimes, label='A Priori')
    plt.plot(supports, fpTimes, label='FP-Tree')
    plt.xlabel('Support threshold in Percentage')
    plt.ylabel('Time taken(in log)')
    plt.title('Execution time vs support for Apriori and FP-tree')
    plt.legend(loc='upper right')
    plt.show()
    plt.savefig('graph-log.pdf')


if __name__ == '__main__':

    inputFile = sys.argv[1]

    supports=[1, 5, 10, 25, 50, 90]

    apTimes = []
    for support in supports:
        t = timeit(stmt="os.system('./apriori {} {} output-apriori')".format(inputFile, support), setup="import os", number=1)
        apTimes.append(t)
    
    fpTimes = []
    for support in supports:
        t = timeit(stmt="os.system('./fptree {} {} output-fptree')".format(
            inputFile, support), setup="import os", number=1)
        fpTimes.append(t)

    plot(apTimes,fpTimes,supports)
    plotLog(apTimes,fpTimes,supports)