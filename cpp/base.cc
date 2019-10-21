#include <iostream>
#include <fstream>
#include <exception>
#include <typeinfo>

#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include <type_traits>
#include <typeinfo>
#include <ctime>

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "chrono_util.h"

using namespace std;


// final指定类不能被继承, 也可作用于方法上
struct Foo final {
  Foo() : v(0) {
    cout << "con foo default" << endl;
  }
  Foo(int v) : v(v) {
    cout << "con foo " << v << endl;
    //this->v = v;
  }
  ~Foo() {
    cout << "decon foo " << v << endl;
  }
  Foo(const Foo& obj) {
    cout << "copy-con foo " << obj.v << endl;
    this->v = obj.v;
  }
  int v;
};


std::chrono::duration<int, std::milli> buildDurationMs(int ms) {
  return std::chrono::duration<int, std::milli>(ms);
}


// #define RUN testFutureAndPromise
void testFutureAndPromise() {
  promise<int> p;

  future<int> f = p.get_future();
  future_status st = f.wait_for(buildDurationMs(3000));  // 3秒
  cout << (st == future_status::timeout) << endl;

  p.set_value(0);
  st = f.wait_for(buildDurationMs(30000));
  cout << (st == future_status::ready) << endl;

  int v = f.get();
  cout << v << endl;
}
/*
  enum future_status {
    ready,
    timeout,
    deferred
  };
*/


// #define RUN testChrono
void testChrono() {
  // sleep
  std::chrono::seconds sec3(3);
  auto t = std::time(0);
  std::this_thread::sleep_for(sec3);
  cout << "sleep: " << std::time(0) - t << endl;  // 3
}


// #define RUN testStringStream
void testStringStream() {
  stringstream s;
  s.str("");
  s << "abc";
  s << "\t";
  s << 123;
  cout << s.str() << endl;
  cout << s.str().capacity() << endl;

  s.str("");
  cout << s.str() << endl;
  cout << s.str().capacity() << endl;

  // 复用
  s << "def";
  s << "\t";
  s << 456;
  cout << s.str() << endl;
  cout << s.str().capacity() << endl;
}


// #define RUN testBoostUUID
void testBoostUUID() {
  auto start_tm = ChronoUtil::now();
  for (int i = 0; i < 10; i++) {
    string s = boost::uuids::to_string(boost::uuids::random_generator()());
  }
  auto end_tm = ChronoUtil::now();
  auto cost = ChronoUtil::timeDiff(end_tm, start_tm);
  std::cout << "cost: " << cost << std::endl;  // 22
  // boost获取uuid平响2ms, 性能差, 不要使用
}


// #define RUN testString
void testString() {
  std::string s = "axc";
#if __cplusplus > 201402L
  // c++17 开始新增data()返回非const, c_str()依然返回const
  char* d = s.data();
  d[1] = 'b';
#endif
  cout << s << endl;  // 对于 c++17, 得到 abc
}


// #define RUN testCountDownLatch
class CountDownLatch {
public:
  CountDownLatch(int32_t n) : num_(n) {}

  void wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    cond_.wait(lk, [this] { return this->num_ <= 0; });
  }

  void countDown(int32_t n) {
    if (num_ <= 0) {
      return;
    }
    if (num_.fetch_add(-n) <= n) {
      cond_.notify_all();
    }
  }

private:
  std::atomic<int32_t> num_;
  std::mutex mtx_;
  std::condition_variable cond_;
};
void testCountDownLatch() {
  CountDownLatch latch(2);
  thread t1([](CountDownLatch& latch) {
    cout << "t1 start\n";
    latch.wait();
    cout << "t1 end\n";
  }, std::ref(latch));
  thread t2([](CountDownLatch& latch) {
    cout << "t2 start\n";
    latch.wait();
    cout << "t2 end\n";
  }, std::ref(latch));

  latch.countDown(1);
  cout << "main sleep\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  cout << "main wake\n";
  latch.countDown(1);

  t1.join();
  t2.join();
}


// #define RUN testNullReference
void testNullReference() {
  Foo* p = static_cast<Foo*>(nullptr);
  Foo& r = *p;  // 此时并没有访问p所指内存(没有读写操作)
  cout << reinterpret_cast<uint64_t>(&r) << endl;
}


// #define RUN testSharedPtrForArray
void testSharedPtrForArray() {
  Foo* foos = new Foo[2];
  shared_ptr<void> ptr(nullptr, [foos](void* p) {
    cout << reinterpret_cast<uint64_t>(p) << endl;  // 0
    delete[] foos;
  });
}


// #define RUN test_ConstIterator_And_IteratorConst
void test_ConstIterator_And_IteratorConst() {
  list<int> l;
  l.push_back(1);
  l.push_back(2);
  list<int>::const_iterator c_it = l.begin();  // read only
  cout << *c_it << ", " << *(++c_it) << endl;

  const list<int>::iterator it = l.begin();
  *it = 10;
  // *(++it) = 20;  error
  cout << *l.begin() << ", " << *(++l.begin()) << endl;

  list<int>::iterator const it1 = l.begin();
  *it1 = 100;
  // *(++it1) = 200; error
  cout << *l.begin() << ", " << *(++l.begin()) << endl;
}


// #define RUN testStaticVarInFunction
struct TestStaticVarInFunction {
  TestStaticVarInFunction() {
    cout << "construct" << endl;
  }
};
void testStaticVarInFunctionRun() {
  cout << "test start" << endl;
  static TestStaticVarInFunction v;
  cout << "test end" << endl;
}
// static变量延迟初始化, 只是在bss段分配了内存空间
void testStaticVarInFunction() {
  testStaticVarInFunctionRun();
}


// #define RUN testTypeidName
struct TypeidTest {
  template<typename T>
  void print(T& t) {
    cout << typeid(t).name() << ": " << t << endl;
  }
};
void testTypeidName() {
  TypeidTest a;
  cout << typeid(a).name() << endl;  // 10TypeidTest

  int i = 1;
  a.print<int>(i);  // i

  string s("typeidtest");
  a.print<string>(s);  // NSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE
}


// #define RUN testVectorExpandCapacity
// vector扩容时调用copy-con, 不能直接用memcpy, 因为copy-con里可能执行了其他用户逻辑
void testVectorExpandCapacity() {
  vector<Foo> v;
  for (int i = 1; i <= 8; i++) {
    int old_cap = v.capacity();
    v.emplace_back(i);
    cout << "  " << old_cap << ", " << v.capacity() << endl;
  }
}


// #define RUN testEnumAndInt
enum Color {
  Red = 1,
  Green,
  Blue,
};
void testEnumAndInt() {
  int x = Color::Blue;  // enum to int
  cout << x << endl;    // 3
  Color c1 = Color(2);  // int to enum
  cout << (c1 == Color::Green) << endl;  // true
}


// #define RUN testCast
struct TestCastBase {
  int v;
};
struct TestCastDerivedA : TestCastBase {
  int a;
};
struct TestCastDerivedB : TestCastBase {
};
struct TestCastDerivedC : TestCastBase {
  int c;
};
void testCast() {
  float x = 1.5;

  // static_cast: 代替隐式转换
  int y = static_cast<int>(x);
  cout << y << endl;

  TestCastDerivedA derivedA;
  derivedA.v = 7;
  derivedA.a = 17;
  TestCastBase* basePtr = &derivedA;
  TestCastDerivedC* derivedPtr = static_cast<TestCastDerivedC*>(basePtr);
  // v=7, c=17
  cout << "cast A to C. v=" << derivedPtr->v << ", c=" << derivedPtr->c << endl;
  TestCastDerivedB derivedB;
  derivedB.v = 11;
  basePtr = &derivedB;
  derivedPtr = static_cast<TestCastDerivedC*>(basePtr);
  // v=11, c=1
  cout << "cast B to C. v=" << derivedPtr->v << ", c=" << derivedPtr->c << endl;

  // reinterpret_cast<T>: 无损转换, T是指针/引用/整型, 用于指针和整型间的无损转换
  // int y1 = reinterpret_cast<int>(x);  编译报error
  uint64_t z = reinterpret_cast<uint64_t>(&x);
  float x1 = *reinterpret_cast<float*>(z);
  int x_int = *reinterpret_cast<int*>(z);
  cout << z << ", " << x1 << ", " << x_int << endl;
}


// #define RUN testDynamicCast
struct TestDynamicCast_A {
  virtual void Do() {}  // 必须是多态才能使dynamic_cast做运行时检查
};
struct TestDynamicCast_B : public TestDynamicCast_A {
};
struct TestDynamicCast_X {
  virtual void Do() {}
};
// 向上转可以static_cast代替, 向下转需用dynamic_cast做检查
void testDynamicCast() {
  TestDynamicCast_B b, *pb(&b);
  TestDynamicCast_A a, *pa(&a);
  TestDynamicCast_A* pab = static_cast<TestDynamicCast_A*>(&b);
  bool r;

  r = dynamic_cast<TestDynamicCast_A*>(pb) != nullptr;  // up
  cout << r << endl;  // true

  r = dynamic_cast<TestDynamicCast_B*>(pab) != nullptr;  // down
  cout << r << endl;  // true

  r = dynamic_cast<TestDynamicCast_B*>(pa) != nullptr;  // down
  cout << r << endl;  // false

  TestDynamicCast_X x;
  r = dynamic_cast<TestDynamicCast_A*>(&x) != nullptr;
  cout << r << endl;  // false
}


// #define RUN literalNum
// 用户自定义字面值
long long operator"" _k(unsigned long long x) { return x * 1000; }
long long operator"" _K(unsigned long long x) { return (x << 10); }
long double operator"" _k(long double x) { return x * 1000; }
size_t operator"" _len(const char* s, size_t size) { return size; }  // size参数自动推断
void literalNum() {
  cout << 2_k << endl;  // 2000
  cout << 2_K << endl;  // 2048
  cout << 2.6_k << endl;  // 2600
  cout << "123"_len << endl;  // 3
}


// #define RUN appendStr
class StrAppender {
 public:
  StrAppender() { strs.reserve(2); }
  StrAppender(const string&& first) {
    strs.reserve(2);
    if (!first.empty()) strs.push_back(first);
  }
  StrAppender& Append(const string&& s) {
    if (!s.empty()) strs.push_back(s);
    return *this;
  }
  string ToString() {
    string res;
    for (const auto& s : strs) {
      res += s;
    }
    return res;
  }
 private:
  vector<const string> strs;
};
void appendStr() {
  string res = StrAppender("1").Append("2").Append("3").ToString();
  cout << res << endl;
}


// #define RUN iter
// 支持 for(e : lst)的迭代器
template <typename T>
class Container {
 public:
  template<typename E>
  class Iter : public iterator<input_iterator_tag, E> {
   private:
    Iter(Container<E>* container) : container_(container), pos_(0) {}
    Iter(Container<E>* container, uint32_t pos) : container_(container), pos_(pos) {}
    friend class Container;
   public:
    bool operator ==(const Iter& other) {
      return pos_ == other.pos_ &&
             static_cast<void*>(container_) == static_cast<void*>(other.container_);
    }
    bool operator !=(const Iter& other) {
      return !(*this == other);
    }
    Iter& operator ++() {
      ++pos_;
      return *this;
    }
    E& operator *() {
      return container_->get(pos_);
    }
   private:
    Container<E>* container_;
    uint32_t pos_;
  };

  typedef Iter<T> iterator;

  Container(const vector<T>& input) : data_(input) {}

  iterator begin() {
    return Iter<T>(this);
  }
  iterator end() {
    return Iter<T>(this, data_.size());
  }
  T& get(uint32_t pos) {
    return data_[pos];
  }

 private:
  vector<T> data_;
};
void iter() {
  Container<string> strs({"1", "2", "3"});
  for (string& e : strs) {
    cout << e << endl;
  }
}


// #define RUN catchException
void catchException() {
  try {
    auto x = 0;
    auto y = 1 / x;
  } catch (const std::exception& ex) {
    cout << ex.what() << endl;
  }
}


int main() {
  std::cout << std::boolalpha;
  std::cout << "----------" << std::endl;
  RUN();
  std::cout << "----------" << std::endl;
}