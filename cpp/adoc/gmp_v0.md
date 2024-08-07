#
分支: tag_go1.0.3_dev
根目录: src/pkg/runtime
ulimit -s  # 查看线程栈大小(单位KB), 一般是8M
ulimit -s 10240  # 设置成10M

# runtime.h
~~~c++
// goroutine结构体定义
struct G {
  byte*  stackguard;    // 栈结束地址 低地址
  byte*  stackbase;     // 栈起始地址 高地址
  ...
  int16  status;
  ...
};
~~~

# proc.c
~~~c++
struct Sched {
  Lock l;

  G* gfree; // available g's (status == Gdead)
  int32 goidgen;

  G* ghead; // g's waiting to run
  G* gtail;
  int32 gwait;    // number of g's waiting to run
  int32 gcount;   // number of g's that are alive
  int32 grunning; // number of g's running on cpu or in syscall

  M* mhead;     // m's waiting for work
  int32 mwait;  // number of m's waiting for work
  int32 mcount; // number of m's that have been created

  std::atomic_uint32 mcpu;

  int32 profilehz; // cpu profiling rate

  bool init;     // running initialization
  bool lockmain; // init called runtime.LockOSThread
};

// 全局调度器
Sched runtime_sched;

void schedule(G* gp) {
  runtime_sched.l.lock();

  if (gp != nullptr) {
    gp->m = nullptr;
    runtime_sched.grunning--;
    runtime_sched.mcpu--;
    if (gp->status == Grunnable || gp->status == Gdead) {
      throw std::runtime_error("bad gp->status in sched");
    }
    else if (gp->status == Grunning) {
      gp->status = Grunnable;
      gput(gp);
    }
  }
}
~~~
