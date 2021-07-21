#! /bin/bash

rm -f *.pb.h
rm -f *.pb.cc
protoc *.proto --cpp_out=.

rm -f a.out
rm -rf a.out.dSYM
rm -f core

ulimit -c unlimited

cpp="g++ -std=c++11 \
    -Werror -Wall -Wno-unused-variable -g -O3 \
    -I. -I.. -I/usr/local/include"

libs="-lprotobuf -lpthread"

srcs=`ls ../util/*.cc`

if [ $# -lt 1 ]; then
  srcs="test_user.cc $srcs"
else
  srcs="$1 $srcs"
fi
$cpp $srcs $libs 2>&1

if [ -f a.out ]; then
  ./a.out
fi
