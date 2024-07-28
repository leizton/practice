/*
分支: tag_go1.0.3_dev
根目录: src/pkg/runtime
ulimit -s  # 查看线程栈大小(单位KB), 一般是8M
ulimit -s 10240  # 设置成10M
*/

// goroutine 结构体. runtime.h
struct G {
  byte*  stackguard;    // 栈结束地址 低地址
  byte*  stackbase;     // 栈起始地址 高地址
  // ...
  int16  status;
  // ...
  G* schedlink;
	bool readyonstop;
  bool ispanic;
  M* m;                 // for debug
  M* lockedm;
  M* idlem;
};

// machine 结构体. runtime.h
struct M {
  G* g0;		// goroutine with scheduling stack
  // ...
	int32	waitnextg;
  // ...
	Note havenextg;
  G* nextg;
  // ...
  FixAlloc* stackalloc;
};

// 调度器. proc.c
struct Sched {
  Lock l;

  G* gfree = nullptr; // available g's (status == Gdead)
  int32 goidgen;

  G* ghead = nullptr; // g's waiting to run
  G* gtail = nullptr;

  int32 gwait;    // number of g's waiting to run
  int32 gcount;   // number of g's that are alive
  int32 grunning; // number of g's running on cpu or in syscall

  M* mhead = nullptr; // m's waiting for work
  int32 mwait;        // number of m's waiting for work
  int32 mcount;       // number of m's that have been created

  std::atomic_uint32 mcpu;
  std::atomic_uint32 gwaiting;

  int32 profilehz; // cpu profiling rate

  bool init;     // running initialization
  bool lockmain; // init called runtime.LockOSThread

  void GqueueAdd(G* g) {
    g->schedlink = nullptr;
    if (ghead == nullptr) ghead = g;
    else gtail->schedlink = g;
    gtail = g;
  }
  G* GqueuePop() {
    if (ghead == nullptr) return nullptr;
    G* g = ghead;
    ghead = g->schedlink;
    if (ghead == nullptr) gtail = nullptr;
    return g;
  }
};

// 全局变量. proc.c
static Sched runtime_sched;
static M* mwakeup = nullptr;
// runtime.h
extern register G* global_g;
extern register M* global_m;

// stack.h
// https://www.cnblogs.com/luozhiyun/p/14619585.html
enum {
	StackSystem = 0,     // 操作系统预留size
	StackTop    = 72,    // 栈创建后预留size, 这部分不给用户栈使用. @ref proc.c::runtime_malg
	StackMin    = 4096,  // 栈最小size
	StackSmall  = 128,   // 用于小函数优化
	StackBig    = 4096,  // 函数开始调用时预计需要的栈size超过该值, 则调用 runtime_morestack 扩容
	StackExtra  = 1024,  // 栈扩容时扩多少量
	StackGuard  = 256 + StackSystem,  // 当栈空间剩余size小于等于该值时扩容
	FixedStack  = StackMin + StackSystem,
	StackLimit  = StackGuard - StackSystem - StackSmall,  // 不允许栈分裂的函数(NOSPLIT function)所能使用的最大栈size
};

// proc.c
void schedule(G* g) {
  runtime_sched.l.lock();
  if (g != nullptr) {
    g->m = nullptr;
    runtime_sched.grunning--;
    runtime_sched.mcpu--;

    if (g->status == Grunnable || g->status == Gdead) {
      throw std::runtime_error("bad g->status in sched");
    }
    else if (g->status == Grunning) {
      g->status = Grunnable;
      gput(g);
    }
    else if (g->status == Gmoribund) {
      g->status = Gdead;
      // g 和 m 解绑
      if (g->lockedm) {
        g->lockedm = nullptr;
        m->lockedg = nullptr;
      }
      g->idlem = nullptr;
      unwindstack(g, nil);  // 清空gp的所有调用栈
      gfput(g);  // 放到 runtime_sched.gfree 链表上后台清理
      if (--runtime_sched.gcount == 0) runtime_exit(0);
    }
    if (g->readyonstop) {
      g->readyonstop = 0;
      readylocked(g);
    }
  } else if (global_m->helpgc) {
    runtime_sched.mcpu--;
    runtime_sched.grunning--;
    global_m->helpgc = 0;
  } else if (global_m->nextg != nil) {
    // New m started by matchmg.
  } else {
    throw std::runtime_error("invalid m state in scheduler");
  }
  g = nextgandunlock();  // 找下一个可执行的g
  g->readyonstop = 0;
  g->status = Grunning;
  global_m->curg = g;
  g->m = global_m;
}

// g加到全局任务队列. proc.c
void gput(G* g) {
  if (g->m) {  // 已经绑定 m
    runtime_sched.mcpu++;
    mnextg(g->m, g);
  } else {
    runtime_sched.GqueueAdd(g);
    if (runtime_sched.gwait++ == 0) runtime_sched.gwaiting++;
  }
}

// 从全局队列取任务g. proc.c
G* gget() {
  G* g = runtime_sched.GqueuePop();
  if (g) {
    if (--runtime_sched.gwait == 0) runtime_sched.gwaiting--;
  } else {
    g = global_m->idleg;
    if (g) global_m->idleg = nullptr;
  }
  return g;
}

// 从全局队列取线程m. proc.c
M* mget(G* g) {
  if (g && g->lockedm) return g->lockedm;
  M* m = runtime_sched.mhead;
  if (m) {
    runtime_sched.mhead = m->schedlink;
    runtime_sched.mwait--;
  }
  return m;
}

// 创建新的线程m. proc.c
M* runtime_newm() {
  M* m = new M;
  m->id = runtime_sched.mcount++;
  m->stackalloc = new FixAlloc;
  m->stackalloc->size = 4096;
  m->g0 = runtime_malg(8192);
  runtime_newosproc(m, m->g0, m->g0->stackbase, runtime_mstart);
  return m;
}
void runtime_malg(int stack_size) {
  stack_size += StackSystem;
  byte* sk = new byte[stack_size];
  if (global_g != global_m->g0) {
    global_m->g0->param = sk;
    runtime_gogo(&global_m->g0->sched, 0);
    global_g = nullptr;
  }
  G* newg = new G;
  newg->stack0 = sk;  // 栈上最小地址
  newg->stackguard = sk + StackGuard;
  newg->stackbase = sk + stacksize - sizeof(Stktop);  // 预留Stktop大小的空间
  memset(newg->stackbase, 0, sizeof(Stktop));
  return newg;
}

// g交给m执行. proc.c
void mnextg(M* m, G* g) {
  runtime_sched.grunning++;
  m->nextg = g;
  if (m->waitnextg) {
    m->waitnextg = 0;
    if (mwakeup) {
      mwakeup->havenextg.key = 1;
      ::futex_wake(uaddr=&(mwakeup->havenextg.key), nwake=1);  // 唤醒在锁上等待的nwake个任务
    }
    mwakeup = m;
  }
}

// proc.c
void readylocked(G* g) {
  if (g->m) {
    g->readyonstop = 1;  // 换到其他m执行
    return;
  }
  g->status = Grunnable;
  gput(g);
  matchmg();
}

// proc.c
void matchmg() {
  if (global_m->mallocing || global_m->gcing) {
    return;
  }
  while (runtime_sched.ghead || global_m->idleg) {
    runtime_sched.mcpu++;
    G* g = gget();
    M* m = mget(g);
    if (m == nullptr) m = runtime_newm();
    mnextg(m, g);
  }
}
