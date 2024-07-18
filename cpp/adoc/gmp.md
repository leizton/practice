~~~c++

// 根目录: src/pkg/runtime

// goroutine结构体定义. runtime.h
struct G {
  byte*  stackguard;    // 栈结束地址 低地址
  byte*  stackbase;     // 栈起始地址 高地址
  ...
  int16  status;
  ...
};

// proc.c
void schedule(G* gp) {
}

~~~

ulimit -s  # 查看线程栈大小(单位KB), 一般是8M
ulimit -s 10240  # 设置成10M