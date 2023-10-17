#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

cpp="$gcc_home/bin/g++ -std=c++17 \
    -Werror -Wall -Wno-unused-variable -g -O2 -rdynamic \
    -mavx2 -mpclmul -mbmi \
    -I. -I/usr/local/include \
    -L/usr/local/lib"

libs="-lpthread"

srcs=`ls ./util/*.cc`

if [ $# -lt 1 ]; then
  srcs="main.cc $srcs"
else
  srcs="$1 $srcs"
fi
$cpp $srcs $libs 2>&1 | grep -v 'renamed to -macos_version_min'

if [ -f a.out ]; then
  echo '##########'
  ./a.out
fi
