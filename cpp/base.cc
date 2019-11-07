#include "chrono_util.h"
#include "base.h"
#include "gre_words.h"


#define RUN testMutable
void testMutableUtil(Aoo& a) { a.v = "test-"+to_string(std::rand()); }
struct TestMutableClz {
  void print() const { testMutableUtil(a_); COUT(a_.v); }
  mutable Aoo a_;
};
void testMutable() {
  const TestMutableClz x;
  x.print();
  x.print();
}


// #define RUN testFunctionCopy
void testFunctionCopyUtil1(Aoo a) {
  cout << "util1 " << a.v << endl;
}
void testFunctionCopyUtil2(Aoo& a) {
  cout << "util2 " << a.v << endl;
}
void testFunctionCopyUtil3(Aoo* a) {
  cout << "util3 " << a->v << endl;
}
void testFunctionCopy() {
  Aoo a("a1");
  cout << "\n\n";

  /**
   * 3次拷贝
   * a 进入 bind 是 con&-2
   * bind 返回的临时变量传给 f1 是 con&&-3
   * f1产生后析构临时变量 decon-2
   * f1调用时, 传给 testFunctionCopyUtil1 是 con&-4
   */
  cout << "*************** 1\n";
  function<void()> f1 = bind(testFunctionCopyUtil1, a);
  f1();
  cout << "***************\n\n";

  /**
   * 2次拷贝
   * 省去f2调用时的一次拷贝
   */
  cout << "*************** 2\n";
  function<void()> f2 = bind(testFunctionCopyUtil2, a);
  f2();
  cout << "***************\n\n";

  // 0次拷贝
  cout << "*************** 3\n";
  function<void()> f3 = bind(testFunctionCopyUtil3, &a);
  f3();
  cout << "***************\n\n";

  // 0次拷贝
  cout << "*************** 4\n";
  function<void()> f4 = [&a] {
    testFunctionCopyUtil2(a);
  };
  f4();
  cout << "***************\n\n";
}


// #define RUN testStringJoin
void testStringJoin() {
  const int n = 5000;
  const string s("0123456789");
  //
  uint64_t t1 = nowUs();
  string ret1;
  for (int i = 0; i < n; i++) {
    ret1 = ret1 + s;
  }
  t1 = nowUs() - t1;
  //
  uint64_t t2 = nowUs();
  string ret2;
  for (int i = 0; i < n; i++) {
    ret2 += s;
  }
  t2 = nowUs() - t2;
  //
  cout << t1 << ", " << t2 << endl;  // 5100, 80
}


// #define RUN testDynamicCast
void testDynamicCast() {
  class A { public: virtual ~A(){} };  // A中必须定义一个virtual才能用dynamic_cast
  class B : public A {};
  class C { public: virtual ~C(){} };

  // dynamic_cast<T>
  B b, *pb(&b);
  A a, *pa(&a), *pa1(&b);
  C c, *pc(&c);
  ASSERT_TRUE(dynamic_cast<A*>(pb)  != nullptr);  // 向上转换
  ASSERT_TRUE(dynamic_cast<B*>(pa)  == nullptr);  // 向下转换
  ASSERT_TRUE(dynamic_cast<A*>(pa1) != nullptr);  // 向上转换
  ASSERT_TRUE(dynamic_cast<A*>(pc)  == nullptr);

  // dynamic_pointer_cast<T>
  shared_ptr<A> p1(new B);
  ASSERT_TRUE(p1.use_count() == 1);
  shared_ptr<B> p2 = std::dynamic_pointer_cast<B>(p1);
  ASSERT_TRUE(p1.use_count() == 2);
  ASSERT_TRUE(p2 != nullptr);
}


// #define RUN testSpinLock
void testSpinLock() {
  const int num = 100, thnum = 10;
  int cnt = 0;
  SpinLock lk;

  auto run = [&]{
    for (int i = 0; i < num; i++) {
      lk.lock();
      cnt++;
      sleepUs(1);
      lk.unlock();
    }
  };

  vector<thread> ths;
  for (int i = 0; i < thnum; i++) {
    ths.emplace_back(run);
  }
  for (auto& th : ths) {
    th.join();
  }

  // 没有 lk.lock() 时结果非0
  cout << (num*thnum - cnt) << endl;
}


// #define RUN testGetline
void testGetline() {
  /**
   * 输出
   * abc
   * ab
   * 
   * abcd 加上 \0 超过s的size, 所以getline()返回false
   */
  istringstream in("abc|ab|abcd|a");
  char s[4];
  while (in.getline(s, 4, '|')) {
    cout << s << endl;
  }
}


// #define RUN testSnprintfAndStringStream
void testSnprintfAndStringStream() {
  const int run_num = 200000;
  uint64_t t1, t2;

  t1 = nowMs();
  char s[100];
  for (int i = 0; i < run_num; i++) {
    snprintf(s, 100, "%s %d %ld %f", "abc", 123, 123L, 123.123);  // 77ms
    //snprintf(s, 100, "%s ", "0123456789");  // 18ms
  }
  t1 = nowMs() - t1;

  t2 = nowMs();
  ostringstream ss;
  for (int i = 0; i < run_num; i++) {
    ss.str("");
    ss << "123 " << 123 << " " << 123L << " " << 123.123;  // 149ms
    //ss << "0123456789 ";  // 10ms
  }
  t2 = nowMs() - t2;

  cout << t1 << ", " << t2 << endl;
}


// #define RUN testCurrThreadId
void testCurrThreadId() {
  cout << "main: " << currThreadId() << endl;
  auto th = thread([] {
    cout << "th: " << currThreadId() << endl;
  });
  th.join();
  cout << "main: " << currThreadId() << endl;
}


// #define RUN testMapForeach
void testMapForeach() {
  map<int, shared_ptr<int>> m;
  for (int i = 0; i < 5000; i++) {
    m[i] = make_shared<int>(i);
  }
  uint64_t t1 = nowMs();
  for (int i = 0; i < 1000; i++) {
    for (auto& kv : m) {
      //cout << kv.second.use_count() << endl;  // 1
    }
  }
  uint64_t t2 = nowMs();
  for (int i = 0; i < 1000; i++) {
    for (auto iter = m.begin(); iter != m.end(); ++iter) {
      //cout << iter->second.use_count() << endl;  // 1
    }
  }
  uint64_t t3 = nowMs();
  cout << (t2-t1) << ", " << (t3-t2) << endl;  // 50, 50
}


// #define RUN testOstringstream
void testOstringstream() {
  ostringstream ss;
  ss << 123;
  cout << lengthOfOstringstream(ss) << endl;  // 3
  ss << "abc";
  cout << lengthOfOstringstream(ss) << endl;  // 6
  ss.str("");
  cout << lengthOfOstringstream(ss) << endl;  // 0
}


// #define RUN testMapAndUnorderedMap
void testMapAndUnorderedMap() {
  vector<string> words = getWords();

  map<string, int> m1;
  uint64_t t = nowUs();
  for (auto& w : words) {
    m1[w] = 1;
  }
  for (int i = 0; i < 4; i++) {
    for (auto& w : words) {
      if (m1.find(w) == m1.end()) {
        abort();
      }
    }
  }
  cout << nowUs() - t << endl;  // 9300

  unordered_map<string, int> m2;
  t = nowUs();
  for (auto& w : words) {
    m2[w] = 1;
  }
  for (int i = 0; i < 4; i++) {
    for (auto& w : words) {
      if (m2.find(w) == m2.end()) {
        abort();
      }
    }
  }
  cout << nowUs() - t << endl;  // 3700
}


// #define RUN testConditionVariable
void testConditionVariable() {
  bool completed = false;
  mutex mtx;
  condition_variable cv;

  thread th([&completed, &mtx, &cv]{
    sleepSec(5);
    {
      unique_lock<mutex> lk(mtx);
      completed = true;
      cv.notify_all();  // 必须放在锁里, 防止通知语句被重排到前面
      // 另外, 没有completed变量时也必须用锁包住notify
      // 否则可能出现发通知时, 等待方同时判定成false, 导致跳过这条通知
      // 加锁使notify和判定不会同时发生
    }
    cout << "a----" << nowMs() << endl;
  });
  th.detach();  // 不调用detch则需要调用th.join(), 否则会core
                // main线程退出, 而非daemon线程未结束

  bool done = false;
  auto td = buildDurationMs(800);
  unique_lock<mutex> lk(mtx);
  for (int i = 0; !done; i++) {
    // unique_lock<mutex> lk(mtx);
    done = cv.wait_for(lk, td, [&completed]{ return completed; });
    cout << "b-" << i << "--" << nowMs() << endl;
  }
  lk.unlock();
  cout << "c----" << nowMs() << endl;
}


// #define RUN testSharedPtrDecon
void testSharedPtrDecon() {
  auto b = createObj<Boo>("b");
  auto c = createObj<Coo>("c");
  auto a = createObj<Aoo>("a");
  b->pa = a;
  c->pa = a;
  c->pb = b;
  // 不论 a b c 的创建顺序, 析构顺序都是 c b a
  // 依赖关系通过计数器实现

  // a->pb = b; 加上这句时, a b 都不是被释放
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
  // 平响13us
  auto cost = nowUs();
  const int num = 100;
  for (int i = 0; i < num; i++) {
    string s = boost::uuids::to_string(boost::uuids::random_generator()());
  }
  cost = (nowUs() - cost) / num;
  std::cout << "cost: " << cost << std::endl;  // 13
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
    cout << y << endl;
  } catch (const std::exception& ex) {
    cout << ex.what() << endl;
  }
}


atomic<uint32_t> Aoo::id_;
atomic<uint32_t> Boo::id_;
atomic<uint32_t> Coo::id_;


int main() {
  std::cout << std::boolalpha;
  std::cout << "----------" << std::endl;
  RUN();
  std::cout << "----------" << std::endl;
}