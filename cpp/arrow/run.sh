#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core

ulimit -c 0 # unlimited

cpp="g++ -std=c++11 \
    -Werror -Wall -Wno-unused-variable -g \
    -I. -I.. -I/usr/local/include \
    -L/usr/local/lib"

libs="-larrow -larrow_dataset \
      -larrow_bundled_dependencies \
      -lpthread"

srcs='../util/base64.cc'

if [ $# -lt 1 ]; then
  srcs="recall_filter.cc $srcs"
else
  srcs="$1 $srcs"
fi
$cpp $srcs $libs 2>&1

if [ -f a.out ]; then
  if [ $# -lt 1 ]; then
    ./a.out data/recall_filter.csv
  else
    ./a.out $@
  fi
fi
