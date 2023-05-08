--------------------------------------------------------------------------------------------------------------
# INT32_MIN INT32_MAX
~~~c++
#include <cstdint>
INT8_MIN   INT8_MAX   UINT8_MAX
INT16_MIN  INT16_MAX
INT32_MIN  INT32_MAX
INT64_MIN  INT64_MAX
~~~


--------------------------------------------------------------------------------------------------------------
# 函数名 函数指针 类成员函数 std::function
① 函数名可以隐式转成函数指针, 就像数组名可以隐式转成元素类型的指针
~~~c++
void func() {}
void (*pf)() = func;
pf = &func; // 也可以加上&
(*pf)();
printf("%p, %p, %p", func, &func, pf);
~~~
printf打印的3个值是相等的
`pf = func;` 也可以写成 `pf = &func;`;
② 类静态成员函数和普通函数没有区别也可以隐式转成函数指针
③ 类普通成员函数转成函数指针需要带类名作用域
~~~c++
struct A {
  void test();
};
typedef void (A::*TestPtr)();
TestPtr test = &A::test;
A a;
(a.*test)();
~~~
④ 为了方便使用, 把类成员函数转成std::function
~~~c++
A a;
std::function<void()> test = std::bind(&A::test, &a);
test();
~~~


--------------------------------------------------------------------------------------------------------------
# 类拷贝构造和赋值运算符的区别
~~~c++
struct A {
  A() : data(nullptr) {}
  void* data;
};
A a;
A a1(a);  // 拷贝构造
A a2 = a; // 拷贝构造, !!!不是赋值运算符!!!
          // 进入拷贝构造时, data是野指针
A a3;     // 默认构造
a3 = a;   // 赋值运算符。进入operator=时, data已被初始化成nullptr
~~~


--------------------------------------------------------------------------------------------------------------
# 各种callable的性能比较
[@ref callable_perf](../../stl/functional.cc)


--------------------------------------------------------------------------------------------------------------
# static_assert(bool_expr, [message])
编译期检查


--------------------------------------------------------------------------------------------------------------
# 全局变量(global-var) 和 静态变量(static-var)
.cc文件中用static修饰的是静态全局变量, 否则是普通全局变量
静态全局变量只能在当前文件中访问, 就像类/函数的静态变量只能在类/函数里访问
  优点：即使和其他.cc的变量同名也是在不同的内存空间上
全局变量在其他文件中通过extern声明后可以被访问
  缺点：不同.cc的同名全局变量共用同一个内存空间，会相互覆盖
例子
~~~c++
// 不初始化
int ax0;
static int ax1;
// 初始化
int ax2 = 1;
static int ax3 = 1;
// 常量
const int ax4 = 1;
static const int ax5 = 1;

$ nm a.out | grep ax
0000000100008038 B _ax0        -- bss 段
000000010000803c b __ZL3ax1    -- 小写 b 表示仅当前文件可见
0000000100008030 D _ax2        -- data 段, 有初始化的变量
0000000100008034 d __ZL3ax3
0000000100003eb0 r __ZL3ax4    -- 只读数据区
0000000100003eb4 r __ZL3ax5
~~~


--------------------------------------------------------------------------------------------------------------
# map的3种插入方式
~~~c++
map<int, Aoo> d;
//
d.insert({1, a}); // or d.insert(make_pair(1, a));
  ①  make_pair call copy_construct(a) 创建临时对象 a1
  ②  d.insert call move_copy_construct(a1) 创建map内的元素
  ③  destruct(a1)
//
d[2] = a;
  ①  call default construct() 创建map内的元素
  ②  call operator=(a) 初始化map内的元素
//
d.emplace(3, a);
  ①  call copy_construct(a) 创建map内的元素
~~~


--------------------------------------------------------------------------------------------------------------
# vector的插入
~~~c++
vector<Aoo> v;
v.push_back(a);    // call copy_construct(a)
v.emplace_back(a); // call copy_construct(a)
v.emplace_back(std::move(a)); // call move_copy_construct(a)
~~~


--------------------------------------------------------------------------------------------------------------
# 引用和指针
引用的好处: ①不会指向空对象 ②所指不会改变,避免指向混乱 ③语法比指针简单,代码更干净
所以尽最大可能用引用
--
返回类型是引用
  A& mutableA()
  const A& getA()
返回类型是指针, 表示可能返回nullptr
  A* a()
--
可被修改的形参可以是引用
例如, void std::swap(T& a, T& b);


--------------------------------------------------------------------------------------------------------------
# abort() exit() std::terminate()
exit()会调用`int atexit(void (*f)())`注册的函数、对象的析构函数
abort()不会调用这些, send a SIGABRT signal
std::terminate()调用abort()


--------------------------------------------------------------------------------------------------------------
# 内存访问序模型
~~~c++
// 内存访问序种类
enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst,
};
~~~
`std::atomic 默认用 memory_order_seq_cst`
未经同步的并行程序运行结果不可预期(无一致性保证), 同步的方式有atomic和mutex
@ref https://en.cppreference.com/w/cpp/atomic/memory_order
relaxed
  对其他读写操作没有同步和执行顺序的约束, no synchronization or ordering constraints
consume
  ① 用于load. 当前线程中依赖于load变量的读写操作不能重排到load前
     No reads or writes in the current thread dependent on the loaded value can be reordered before this load
  ② 其他正在release这个load变量的线程中, 往当前线程依赖的变量的写结果在当前线程可见
     Writes to data-dependent variables in other threads that release the same atomic variable are visible in the current thread
  > example
    ~~~c++
    std::atomic<int*> guard(nullptr);
    int payload;
    // produce thread A
    payload = 15;
    guard.store(&payload, memory_order_release);
    // consume thread B
    int* p = guard.load(memory_order_consume);
    if (p != nullptr)
      data = *p;
    // 原则① p的读 data的写 不会重排到load前
    // 原则② 线程A(release guard的其他线程)中payload的写结果对线程B可见, 即*p是更新后的值
    ~~~
acquire
  ① 用于load. 当前线程中, 读写不能重排到load前 (在RA模型中保证读晚于load)
     No reads or writes in the current thread can be reordered before this load
  ② 其他release这个load变量的线程的所有写结果在当前线程可见 (保证能读到其他线程的写)
     从 writes to data-dependent vars 扩大到 all writes
     All writes in other threads that release the same atomic variable are visible in the current thread
release
  ① 用于store. 当前线程中, 读写不能重排到release后 (在RA模型中保证写早于release)
     No reads or writes in the current thread can be reordered after this store
  ② 当前线程的所有写结果, 在其他acquire这个release变量的线程中可见 (保证写对其他线程可见)
     All writes in the current thread are visible in other threads that acquire the same atomic variable
acq_rel
  ① 用于read-modify-write操作(RMW). 同时具有acq和rel, 即当前线程中, 读写不能重排到acq_rel的前面和后面
     No reads or writes in the current thread can be reordered before or after this store
     RMW: 读同时写, 如 fetch_add exchange(交换) compare_exchange_strong(比较后交换)
  ② All writes in other threads that release the same atomic variable are visible before the modification
     The modification is visible in other threads that acquire the same atomic variable
seq_cst
  所有线程以同一个顺序看到所有的修改结果
  plus a single total order exists in which all threads observe all modifications in the same order
Release-Acquire memory order, RA内存序模型
  store用release, load用acquire
c++11的内存屏障
  ~~~c++
  // RA模型中的 guard.store(&payload, memory_order_release) 等价于
  std::atomic_thread_fence(std::memory_order_release);
  guard.store(&payload, memory_order_relaxed);
  // int* p = guard.load(memory_order_acquire) 等价于
  std::atomic_thread_fence(std::memory_order_acquire);
  int* p = guard.load(memory_order_relaxed)
  ~~~
SpinLock
  [spin_lock.h](../../util/spin_lock.h)
double check
~~~c++
class Aoo {
 public:
  static Aoo* instance() {
    Aoo* tmp = instance_.load(std::memory_order_consume);
    if (tmp == nullptr) {
      std::lock_guard<std::mutex> lk(instance_mtx_);
      tmp = instance_.load(std::memory_order_consume);
      if (tmp == nullptr) {
        tmp = new Aoo;
        instance_.store(tmp, std::memory_order_release);
      }
    }
    return tmp;
  }
 private:
  static std::atomic<Aoo*> instance_;
  static std::mutex instance_mtx_;
};
std::atomic<Aoo*> Aoo::instance_{nullptr};
std::mutex Aoo::instance_mtx_;
~~~
@ref
https://www.boost.org/doc/libs/1_56_0/doc/html/atomic/usage_examples.html
https://github.com/apache/incubator-brpc/blob/master/docs/cn/atomic_instructions.md
boost lock free queue(MPMC): https://www.boost.org/doc/libs/1_54_0/doc/html/boost/lockfree/queue.html


--------------------------------------------------------------------------------------------------------------
# atomic operation
① T exchange(T val, memory_order sync = memory_order_seq_cst)
  把atomic的值修改成val, 并返回修改前的值
  {
    T old = v_;
    v_ = val;
    return old;
  }


--------------------------------------------------------------------------------------------------------------
# 多重继承和虚继承
虚继承的作用是解决多重继承中的二义性
第一层的多个子类继承了基类的数据成员
第二层子类从不同的第一层子类中继承了多个同名的基类数据成员, 访问时就会出现二义性
所以, 让第一层子类虚继承基类, 使得数据只有一份

当第一层子类实现了基类的同一个虚函数时, 第二层子类必须重写这个虚函数, 否则会出现歧义(编译失败)
~~~c++
class A { virtual void fn() {} };
class B : virtual public A { virtual void fn() override {} };
class C : virtual public A { virtual void fn() override {} };
class D : public B, public C { virtual void fn() override {} };
~~~

当基类没有定义数据成员时, 第一层子类依然需要虚继承, 因为会默认插入一个字节


--------------------------------------------------------------------------------------------------------------
# type-cast
① static_cast      :  隐式类型转换, 如 int 2 float, `A* 2 void*`
                       强制类型转换, 非多态的基类和子类转换
② const_cast       :  去const修饰
③ dynamic_cast     :  只能用于多态场景, 基类至少有一个virtual函数, 虚析构也行, 否则就用static_cast
                       向下(子类)或向上(基类)转型失败时, 返回nullptr
                       如果是转成引用 dynamic_cast<Base&>, 则抛std::bad_cast异常
④ reinterpret_cast :  dangerous cast, 如 `void* 2 A*`, A* 2 uint64_t, char* 2 int*


--------------------------------------------------------------------------------------------------------------
# 类之间的函数只有继承和隐藏, 没有重载
~~~c++
struct Base {
  void fn(int x);
  void fn(double x);
};
struct Aoo : public Base {
  void fn(const std::string& s) {
    Base::fn(1.0);
  }
};
~~~
Aoo::fn 隐藏了 Base::fn, 因此 Aoo::fn 与 Base::fn 不构成重载
Aoo 中调用 Base::fn 时必须加作用域名


--------------------------------------------------------------------------------------------------------------
# double check lock (DCL) 的问题
https://www.infoworld.com/article/2074979/double-checked-locking--clever--but-broken.html
~~~java
Resource get() {
  if (res == null)
    synchronized
      if (res == null) res = new Resource();
  return res;
}
~~~
res = new Resource(); 实际是两步, synchronized保证这两步的互斥
  1. res = malloc(sizeof(Resource));
  2. new (res) Resource();
  执行完第1步后, 对于另一个线程来说, res`可能`已经不是null, 这就导致另一个线程返回没有经过构造的res
解决方法:
  通过内存屏障保证构造函数调用先于(happens-before)检查成立
  @ref guava::MemoizingSupplier
  ~~~java
  volatile bool inited = false;
  Resource get()
    if (!inited)
      synchronized
        if (!inited)
          var tmp = new Resource();
          res = tmp;
          inited = true; // 后于构造函数调用
          return tmp;
    return res;
  }
  ~~~
PS:
  synchronized、mutex除了有排他执行功能外, 还有就是进入的读屏障、出去的写屏障
  保证进入后读到main-memory的最新值, 出去后写回main-memory


--------------------------------------------------------------------------------------------------------------
~~~cpp
#define likely(cond)   __builtin_expect((cond), 1)
#define unlikely(cond) __builtin_expect((cond), 0)

static uint32_t log_cnt = 0;
if (likely(++log_cnt % 100 == 0)) {
  LOG(INFO) << "print";
}
~~~