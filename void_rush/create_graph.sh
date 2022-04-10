#!/bin/bash

clang++ -w -std=c++17 main.cpp -ogen_test 

./gen_test

neato -n -Tpng -o graph.png graph.dot
