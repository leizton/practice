# -fno-omit-frame-pointer

Don't keep the frame pointer in a register for functions that don't need one.
This avoids the instructions to save, set up and restore frame pointers;
it also makes an extra register available in many functions.
It also makes debugging impossible on some machines.

不把 frame_pointer 存到寄存器里

简单的使用perf record -g 获取到的调用栈是有问题的，存在大量 [Unknown] 函数
从 perf report 的结果来看这些部分对应地址大部分都是非法地址，且生成的火焰图中存在很多明显与代码矛盾的调用关系

# perf fp
仔细阅读 perf record 的手册后发现，perf 同时支持 3 种栈回溯方式：fp, dwarf, lbr
通过 --call-graph 参数指定，而 -g 就相当于 --call-graph fp
fp 就是 Frame Pointer，即 x86 中的 ebp 寄存器
fp 指向当前栈帧栈底地址，此地址保存着上一栈帧的 ebp 值

# perf dwarf
dwarf 是一种调试文件格式
GCC 编译时附加的 -g 参数生成的就是 dwarf 格式的调试信息
调试信息包含了栈回溯所需的全部信息，使用 libunwind 即可展开这些信息
值得一提的是，GDB 进行栈回溯时使用的正是 dwarf 调试信息
实际测试表明使用 dwarf 可以很好的获取到准确的调用栈

# perf lbr
lbr 即 Last Branch Records
是较新的 Intel CPU 中提供的一组硬件寄存器，其作用是记录之前若干次分支跳转的地址
主要目的就是用来支持 perf 这类性能分析工具
此方法是性能与准确性最高的手段，然而它存在一个很大的局限性，
由于硬件 Ring Buffer 寄存器的大小是有限的，lbr 能记录的栈深度也是有限的
具体值取决于特定 CPU 实现，一般就是 32 层

@ref https://gaomf.cn/2019/10/30/perf_stack_traceback/