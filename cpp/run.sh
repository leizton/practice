#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

cpp="$gcc_home/bin/g++ -std=c++20 \
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

compile_start_ts=$(python -c "import time;print(int(time.time()*1000))")
$cpp $srcs $libs 2>&1 | grep -v 'renamed to -macos_version_min'
compile_cost=$(python -c "import time;print(int(time.time()*1000-int($compile_start_ts)))")
echo "compile cost $compile_cost ms"

if [ -f a.out ]; then
  echo '##########'
  ./a.out
fi
