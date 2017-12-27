#! /bin/bash
clear
rm -f a.out

include_dir=../include
if [ $# -gt 0 ]; then
    g++ -std=c++14 -I$include_dir $@ -lpthread
else
    g++ -std=c++14 -I$include_dir main.cc -lpthread
fi

if [ -f a.out ]; then
    ./a.out
fi