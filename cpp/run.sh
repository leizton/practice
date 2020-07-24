#! /bin/bash
rm -f a.out

CPP='g++ -std=c++11 -Werror -Wall -Wno-unused-variable -g -O2 -I.'

SRCS='util/base64.cc'

if [ $# -lt 1 ]; then
  $CPP main.cc $SRCS -pthread
else
  $CPP "$1" $SRCS -pthread
fi

if [ -f a.out ]; then
  ./a.out
fi

rm -rf a.out*
