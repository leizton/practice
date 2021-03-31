https://abcdabcd987.com/libstdc++-bug/

gcc 8 前 std::thread catch了全部异常, 再调用 std::terminate() 引发core
所以 gdb coredump 看不到自己的栈
gcc 8 修复了这个问题
代码文件: libstdc++-v3/src/c++11/thread.cc
```c++
try {
  //
} catch (...) {
  std::terminate();
}
```

gcc内置了unwind库, 用于抛异常时回溯到catch的栈, 同时清理退出的栈
匹配不到catch时, 调terminate(), terminate()内会调abort() 引发coredump
所以gcc是有能力向std::exception提供栈信息
