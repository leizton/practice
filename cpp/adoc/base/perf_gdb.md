sudo perf top -p $PID -g
编译时加上 -fno-omit-frame-pointer

为了防止 `value has been optimized out`
-g -O0 -DSF_VISIBILITY -fvisibility=hidden -fno-strict-aliasing