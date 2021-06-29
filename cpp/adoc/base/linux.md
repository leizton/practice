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
# perf
sudo perf top -p $PID -g
编译时加上 -fno-omit-frame-pointer
测试时 -g -O0 -DSF_VISIBILITY -fvisibility=hidden -fno-strict-aliasing