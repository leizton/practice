#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

compile_start_ts=$(python -c "import time;print(int(time.time()*1000))")

$gcc_home/bin/g++ -std=c++20 -I. main.cc 2>&1 | grep -v 'renamed to -macos_version_min'

compile_cost=$(python -c "import time;print(int(time.time()*1000-int($compile_start_ts)))")
echo "compile cost $compile_cost ms"

if [ -f a.out ]; then
  echo '##########'
  ./a.out
fi
