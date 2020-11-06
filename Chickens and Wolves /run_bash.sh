#!/bin/bash


    g++ main.cpp -o pro1
    ./pro1 start3.txt goal3.txt bfs output_bfs.txt
    ./pro1 start3.txt goal3.txt dfs output_dfs.txt
    ./pro1 start3.txt goal3.txt iddfs output_iddfs.txt
    ./pro1 start3.txt goal3.txt astar output_astar.txt
