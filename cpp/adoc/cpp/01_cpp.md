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
.cc文件中用static修饰的是静态变量, 否则是全局变量
静态变量只能在当前文件中访问, 就像类/函数的静态变量只能在类/函数里访问
全局变量在其他文件中通过extern声明后可以被访问


--------------------------------------------------------------------------------------------------------------
# map的3种插入方式
~~~c++
map<int, Aoo> d;
//
d.insert({1, a}); // or d.insert(make_pair(1, a));
  ⒈  make_pair call copy_construct(a) 创建临时对象 a1
  ⒉  d.insert call move_copy_construct(a1) 创建map内的元素
  ⒊  destruct(a1)
//
d[2] = a;
  ⒈  call default construct() 创建map内的元素
  ⒉  call operator=(a) 初始化map内的元素
//
d.emplace(3, a);
  ⒈  call copy_construct(a) 创建map内的元素
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
    memory_order_relaxed, // no sync of side effects
    memory_order_consume, // sync the visible side effects on values carrying dependencies from the last release or seq_cst
    memory_order_acquire, // sync all visible side effects from the last release or seq_cst
    memory_order_release, // sync side effects with the next consume or acquire
    memory_order_acq_rel, // read as acquire and write as release
    memory_order_seq_cst, // sequentially consistent
};
~~~
std::atomic 默认用 memory_order_seq_cst
未经同步的并行程序运行结果不可预期(无一致性保证), 同步的方式有atomic和mutex
① 各类内存序的happens-before
  release: release前代码的读写不会晚于release执行, 且前面代码的写在release后可见
  acquire: acquire后代码的读写不会早于acquire执行
② Release-Acquire memory order
RA内存序模型的使用方法
  store用release, load用acquire
RA模型的happens-before
  前面的写在store后可见
  load后的读写不会在load前执行
例子
~~~c++
  int ret;  // 最终结果
  int data; // 中间计算的数据
  std::atomic<bool> ready{false}; // 用于同步的变量
  th1() {
    data = 100;
    ready.store(true, std::memory_order_release); // 保证data的可见性
  }
  th2() {
    while (ready.load(std::memory_order_acquire) == false); // 和store同步
    ret = data;
  }
~~~
③ SpinLock
[spin_lock.h](../../util/spin_lock.h)
④ double check
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