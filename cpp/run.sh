#! /bin/bash
rm -f a.out

include_dir=../include
g++ -std=c++11 -g -O0 -I$include_dir $@ -pthread

if [ -f a.out ]; then
    ./a.out
fi
