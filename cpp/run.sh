#! /bin/bash
clear
rm -f a.out

include_dir=../include
g++ -std=c++14 -g -O0 -I$include_dir $@ -lpthread -levent

if [ -f a.out ]; then
    ./a.out
fi