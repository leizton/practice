#! /bin/bash
source ~/.bash_profile
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

cpp="$gcc_home/bin/g++ -std=c++20 \
    -Werror -Wall -Wno-unused-variable -O2 -g3 -rdynamic \
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

start_ts=$(python -c "import time;print(int(time.time()*1000))")
$cpp $srcs $libs 2>&1 | grep -v 'renamed to -macos_version_min'
cost_ms=$(python -c "import time;print(int(time.time()*1000-int($start_ts)))")
echo "compile cost $cost_ms ms"

if [ -f a.out ]; then
  echo '##########'
  start_ts=$(python -c "import time;print(int(time.time()*1000))")
  ./a.out
  echo '##########'
  cost_ms=$(python -c "import time;print(int(time.time()*1000-int($start_ts)))")
  echo "run cost $cost_ms ms"
fi
