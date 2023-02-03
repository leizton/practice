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
# 常用命令
~~~sh
du -s                           # sum 只显示总和
du -h -m -k -b                  # 单位是 人类友好 MB KB byte
du -h --max-depth=1             # 当前目录下各文件的大小, 即 ls -l | awk -F ' ' '{print $9}' | xargs du -hs
du -m --max-depth=1 | sort -n   # 加上排序

awk -F ' ' '{print $NF}'                  # 最后一列
awk -F ' ' '{sum+=$1}END{print sum/NR}'   # 统计平均值
awk -F ' ' '$1 > 400{print $1}'           # 过滤出大于400

sort -n         # 数值
sort -r         # 逆序
sort -nk2 -n5   # 对第2列做数值排序排序, 再对第5列排序
sort -t ","     # 按,分割
sort -u         # 排序后去重

uniq -c   # 去重

## grep
cat line | grep -E "keyword_1|keyword_2"  # grep 两个关键词
cat binary | grep -a  # -a 表示按文本读取 可显示中文

## diff
git diff $file1 $file2  # 用 git 比较更好用
diff -qr $dir1 $dir2    # 比较两个目录, -q 仅显示有无diff 不显示详细, -r 递归子文件

## nc 收发文件
dst=`date +"%Y%m%d_%H%M%S"`;dst="ncfile_$dst";nc -4l 65530 > $dst  # 接收端
    # 先启动接收端
nc $接收端ip 65530 < $ncfile  # 发送端
~~~


--------------------------------------------------------------------------------------------------------------
# perf & gdb

① perf
sudo perf top -p $PID -g
编译选项: -O2 -fno-omit-frame-pointer, -fno-omit-frame-pointer使perf栈是完整的
生成火焰图
  perf record -F 99 -p $pid -m 4 -g -a sleep 5
  perf script > out.perf
  ./FlameGraph/stackcollapse-perf.pl out.perf > out.folded  # https://github.com/brendangregg/FlameGraph
  ./FlameGraph/flamegraph.pl out.folded > pmCount.svg

② gdb
为了防止 `value has been optimized out`
测试时加上 -g -O0 -DSF_VISIBILITY -fvisibility=hidden -fno-strict-aliasing
打印stl
https://daiwk.github.io/posts/knowledge-stack-heap-core.html
~~~sh
mkdir /tmp/tools && cd /tmp/tools
svn co svn://gcc.gnu.org/svn/gcc/trunk/libstdc++-v3/python
vi ~/.gdbinit # 贴入以下代码
python
import sys
sys.path.insert(0, '/tmp/tools/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end
~~~

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


--------------------------------------------------------------------------------------------------------------
# /proc
/proc
  操作系统mount进程空间出来的伪文件, 用于向用户程序提供访问内核数据结构的接口, 大多数文件是只读
  @ref https://man7.org/linux/man-pages/man5/proc.5.html

/proc/self: /proc/$pid 的符号链接

/proc/self/maps: 记录映射式内存空间的信息
  文件格式
  ~~~py
    address("起始和结束地址")   perms  offset    device  inode   pathname
    00e03000-00e24000          rw-p   00000000  00:00   0       [heap]
    00e24000-011f7000          rw-p   00000000  00:00   0       [heap]
    35b1800000-35b1820000      r-xp   00000000  08:02   135522  /usr/lib64/ld-2.15.so
    f2c6ff8c000-7f2c7078c000   rw-p   00000000  00:00   0       [stack:105]  # tid是105的线程的栈空间
    7fffb2c0d000-7fffb2c2e000  rw-p   00000000  00:00   0       [stack]      # main线程的栈空间
  ~~~
  内存映射(mapped memory)
    一段虚拟内存, 把其他内容逐字节映射到当前进程的内存空间
    场景如磁盘文件、网络设备、共享内存(shared_memory_object 进程间通信)
  虚拟内存(VM)
    os给用户进程的是虚拟地址, 通过硬件转成物理地址, 目的是隔离各进程的内存空间, 从而起到保护作用
    另一个目的是实现共享, 包括进程间、用户空间和内核空间, 从而减少拷贝提高性能

/proc/self/mem          进程的内存
/proc/self/cmdline      命令行参数, /proc/self/cmdline | tr '\0' ' ', 分割符是\0
/proc/self/environ      环境变量

--------------------------------------------------------------------------------------------------------------
# nm
nm libmy.so | grep $Symbol
输出有3列
  地址  符号类型  符号名
符号类型
  U      Undefined symbol              链接后找不到 地址是全0
  W      weak symbol           
  V      weak object
  N      debug symbol
  A      global absoulte               该符号的地址是绝对地址，链接过程中不能改变
  T/t    global/local text symbol      text段上
  B/b    global/local bss  symbol      bss段上
  D/d    global/local data symbol      data段上
  L/l    global/static thread local    线程级变量 TLS
@ ref
  https://www.cnblogs.com/baiduboy/p/6048113.html
  https://www.huaweicloud.com/articles/6547604cf65e2f27964f89f39cd0018c.html
