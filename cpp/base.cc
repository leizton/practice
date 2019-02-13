#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <exception>

using namespace std;


struct Foo {
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


// #define RUN testTypeid
struct TestTypeid {
  int x;
};

void testTypeid() {
  int i;
  string s;
  TestTypeid a;
  cout << typeid(i).name() << ", " << typeid(i).hash_code() << endl;  // i
  // NSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE
  cout << typeid(s).name() << ", " << typeid(s).hash_code() << endl;
  cout << typeid(a).name() << ", " << typeid(a).hash_code() << endl;  // 10TestTypeid
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


// 用户自定义字面值
long long operator"" _k(unsigned long long x) { return x * 1000; }
long long operator"" _K(unsigned long long x) { return (x << 10); }
long double operator"" _k(long double x) { return x * 1000; }
size_t operator"" _len(const char* s, size_t size) { return size; }  // size参数自动推断

// #define RUN literalNum
void literalNum() {
  cout << 2_k << endl;  // 2000
  cout << 2_K << endl;  // 2048
  cout << 2.6_k << endl;  // 2600
  cout << "123"_len << endl;  // 3
}


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

// #define RUN appendStr
void appendStr() {
  string res = StrAppender("1").Append("2").Append("3").ToString();
  cout << res << endl;
}


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

// #define RUN iter
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
  cout << boolalpha;
  RUN();
}