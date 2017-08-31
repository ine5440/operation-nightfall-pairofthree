#!/usr/bin/python3
# Auto-tuner prototype
# Built for INE5540 robot overlords

import subprocess # to run stuff
import shlex
import sys # for args, in case you want them
import time # for time

from subprocess import Popen, PIPE
from operator import itemgetter

EXECUTIONS = 10
INPUT_SIZE = 8

def avg(smt):
    return sum(smt)/len(smt)

def run_cmd(arglist, noprint=False):
    if not noprint:
        print(' '.join(arglist))

    begin = time.time()
    subprocess.check_call(args=arglist, stdout=PIPE)
    end = time.time()

    return end - begin


def run(filename):
    arglist = shlex.split(f'./{filename} {INPUT_SIZE}')
    try:
        average = avg([run_cmd(arglist, noprint=True) for _ in range(EXECUTIONS)])
        print(f'Happy {EXECUTIONS} execution(s) in average of {average:.4f}s')
        return average
    except e:
        print(e)
        print('== Sad execution')


def compile(compiler, output, files, flags):
    files = ' '.join(files)
    flags = ' '.join(flags)
    command = f'{compiler} -o {output} {files} {flags}'
    arglist = shlex.split(command)
    ellapsed = run_cmd(arglist)


def tune(filename, level='0', step=8, option=''):
    try:
        compile(compiler='gcc',
                output=filename,
                files=['mm.c'],
                flags=[f'-DSTEP={step}',
                       f'-O{level}',
                       option,
                      ])
    except e:
        print(e)
        print('== Sad compilation')

    return run(filename)


def tuner(argv):
    filename = 'matmult'

    levels = ['0', '1', '2', 's', 'fast']
    steps = [2 ** i for i in range(6)]
    other_options = ['-fopenmp', '-fomit-frame-pointer', '-fno-exceptions', '-march=native']

    average = {}
    for level in levels:
        for step in steps:
            for option in other_options:
                average[(level, step, option)] = tune(filename, level, step, option)

    print(f'Best options: {min(average, key=average.get)}')
    print(f'Sum of average time: {sum(average.values()):.4f}s')


if __name__ == '__main__':
    tuner(sys.argv[1:])
