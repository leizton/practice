#! /bin/bash
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

start_ts=$(python -c "import time;print(int(time.time()*1000))")
$gcc_home/bin/g++ -std=c++20 -I. main.cc 2>&1 | grep -v 'renamed to -macos_version_min'
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