#! /bin/bash
clear
rm -f a.out
if [ $# -gt 0 ]; then
    g++ -std=c++14 -I.. $@
else
    g++ -std=c++14 -I.. main.cc
fi
if [ -f a.out ]; then
    ./a.out
fi