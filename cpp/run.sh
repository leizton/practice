#! /bin/bash
rm -f a.out

include_dir=../include
if [ $# -lt 1 ]; then
  g++ -std=c++11 -Werror -Wall -g -O3 -I$include_dir main.cc -pthread
else
  g++ -std=c++11 -Werror -Wall -g -O3 -I$include_dir $@ -pthread
fi  

if [ -f a.out ]; then
    ./a.out
fi
