#!/bin/bash
cd "/root/Cpp/GreedySnake/" && g++ main.cpp snakebody.cpp map.cpp -g -pthread -o main && ./main && rm main
