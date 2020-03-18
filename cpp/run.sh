#! /bin/bash
rm -f a.out

CPP='g++ -std=c++11 -Werror -Wall -Wno-unused-variable -g -O3 -I.'

if [ $# -lt 1 ]; then
  $CPP main.cc -pthread
else
  $CPP "$1" -pthread
fi

if [ -f a.out ]; then
  ./a.out
fi
