#! /bin/bash
if [ -e ~/.bash_profile ]; then
  source ~/.bash_profile
fi
rm -f a.out
rm -rf a.out.dSYM
rm -f core*

ulimit -c unlimited

if [ $# -lt 1 ]; then
  srcs="main.cc"
else
  srcs="$@"
fi

start_ts=$(python -c "import time;print(int(time.time()*1000))")
$gcc_home/bin/g++ -O2 -g3 -std=c++20 -I. -I/usr/local/include $srcs 2>&1 | grep -v 'renamed to -macos_version_min'
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
