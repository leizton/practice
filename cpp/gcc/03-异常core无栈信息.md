https://abcdabcd987.com/libstdc++-bug/

gcc 8 前 std::thread catch了全部异常, 再调用 std::terminate() 引发core
所以 gdb coredump 看不到自己的栈
```c++
(gdb) bt
#0  0x00007f39d8e435f7 in raise () from /usr/lib64/libc.so.6
#1  0x00007f39d8e44ce8 in abort () from /usr/lib64/libc.so.6
#2  0x00007f39d97486bd in __gnu_cxx::__verbose_terminate_handler ()
    at ../../.././libstdc++-v3/libsupc++/vterminate.cc:95
#3  0x00007f39d9746726 in __cxxabiv1::__terminate (handler=<optimized out>)
    at ../../.././libstdc++-v3/libsupc++/eh_terminate.cc:47
#4  0x00007f39d9746771 in std::terminate ()
    at ../../.././libstdc++-v3/libsupc++/eh_terminate.cc:57
#5  0x00007f39d979c271 in std::(anonymous namespace)::execute_native_thread_routine (__p=<optimized out>)
    at ../../../.././libstdc++-v3/src/c++11/thread.cc:92
#6  0x00007f39d99f7dc5 in start_thread () from /usr/lib64/libpthread.so.0
#7  0x00007f39d8f04ced in clone () from /usr/lib64/libc.so.6
```

gcc 8 修复了这个问题
代码文件: libstdc++-v3/src/c++11/thread.cc
```c++
// 修改前 ----------------------
try {
  __t->_M_run();
} catch (...) {
  std::terminate();
}
// 修改后 ----------------------
  __t->_M_run();
```

gcc内置了unwind库, 用于抛异常时回溯到catch的栈, 同时清理退出的栈
匹配不到catch时, 调terminate(), terminate()内会调abort() 引发coredump
所以gcc是有能力向std::exception提供栈信息
