#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

cpp="$gcc_home/bin/g++ -std=c++11 \
    -Werror -Wall -Wno-unused-variable -g -O3 -rdynamic \
    -I. -I/usr/local/include \
    -L/usr/local/lib"

libs="-lpthread"

srcs='util/base64.cc util/backtrace.cc'

if [ $# -lt 1 ]; then
  srcs="main.cc $srcs"
else
  srcs="$1 $srcs"
fi
$cpp $srcs $libs 2>&1 | grep -v 'libstdc++.dylib'

if [ -f a.out ]; then
  ./a.out
fi
