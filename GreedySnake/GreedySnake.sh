#!/bin/bash
work_path=$(dirname $0)
cd ${work_path}
work_path=$(pwd)
cd ${work_path}
g++ main.cpp snakebody.cpp map.cpp -g -pthread -o main && ./main && rm main
