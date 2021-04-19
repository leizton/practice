#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core

CPP="$gcc_home/bin/g++ -std=c++11 -Werror -Wall -Wno-unused-variable -g -O3 -I."

SRCS='util/base64.cc'

if [ $# -lt 1 ]; then
  $CPP main.cc $SRCS -pthread
else
  $CPP "$1" $SRCS -pthread
fi

if [ -f a.out ]; then
  ./a.out
fi
