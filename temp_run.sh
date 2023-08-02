#!/bin/bash
gcc Louvian.c -o louvian.out -g -fopenmp
./louvian.out graph_info.txt 99
