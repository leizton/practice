--------------------------------------------------------------------------------------------------------------
# signal
SIGINT       2     Interrupt                            中断进程 Ctrl+C
SIGQUIT      3     Quit                                 建立CORE文件终止进程, 并且生成core文件 Ctrl+\
SIGILL       4     Illegal instruction                  建立CORE文件, 非法指令
SIGTRAP      5     Trace trap                           建立CORE文件, 跟踪自陷
SIGABRT      6     Abort                                abort()函数给自己发信号6
SIGIOT       6     IOT trap                             建立CORE文件, 执行I/O自陷
SIGBUS       7     BUS error                            建立CORE文件, 总线错误
SIGFPE       8     Floating-point exception             建立CORE文件, 浮点异常
SIGKILL      9     Kill, unblockable                    杀死进程
SIGUSR1      10    User-defined signal 1                用户定义信号1
SIGSEGV      11    Segmentation violation               建立CORE文件, 段错误, 非法内存访问
SIGTERM      15                                         终止信号
SIGCHLD      17                                         子进程退出


--------------------------------------------------------------------------------------------------------------
# perf & gdb

① perf
sudo perf top -p $PID -g
编译时加上 -fno-omit-frame-pointer 可以使perf栈是完整的

② gdb
为了防止 `value has been optimized out`
测试时加上 -g -O0 -DSF_VISIBILITY -fvisibility=hidden -fno-strict-aliasing


--------------------------------------------------------------------------------------------------------------
# ldd
查看可执行文件运行时依赖的共享库
~~~s
$ ldd test
libstdc++.so.6 => /usr/lib64/libstdc++.so.6 (0x00000039a7e00000)
libm.so.6 => /lib64/libm.so.6 (0x0000003996400000)
~~~
原理:
  实质是通过ld-linux.so(elf动态库的装载器)实现, ld-linux.so先于可执行模块运行并取得控制权
  ldd 让 ld-linux.so 只打印依赖, 不做后续执行

--------------------------------------------------------------------------------------------------------------
# pstack
输出进程当前栈
pstack是一段调用gdb的脚本: cat `which pstack`
~~~s
# gdb命令: thread apply all bt
gdb --quiet -nx /proc/$test_pid/exec $test_pid <<EOF |
thread apply all bt
EOF
/bin/sed -n -e 's/^\((gdb) \)*//' -e '/^#/p' -e '/^Thread/p'
~~~