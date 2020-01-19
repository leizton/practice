#pragma once

#include <cstdlib>
#include <cstdio>
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
#include <queue>
#include <set>
#include <unordered_set>
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

#include "unit_test.h"

using namespace std;


#define CSTR(s) #s
#define VAR_STR(s) std::string s(CSTR(s));


template<class T>
ostream& operator <<(ostream& out, vector<T> v) {
  out << "[";
  bool first = true;
  for (T& e : v) {
    if (!first) out << ",";
    first = false;
    out << e;
  }
  out << "]";
  return out;
}


template<class T>
void operator +=(vector<T>& x, vector<T> y) {
  x.insert(x.end(), y.begin(), y.end());
}


class SpinLock {
public:
  SpinLock() : flag_(false) {}

  void lock() {
    bool expect = false;
    while (!flag_.compare_exchange_strong(expect, true)) {
      // 当cas失败时, expect是true. 所以此处必须置false
      expect = false;
    }
  }

  void unlock() {
    flag_.store(false);
  }

private:
  std::atomic<bool> flag_;
};


// 获取当前线程id
uint64_t currThreadId() {
  return std::hash<std::thread::id>{}(std::this_thread::get_id());
}


// ostringstream的长度
int lengthOfOstringstream(const ostringstream& ss) {
  /**
   * pubseekoff(off, dir, which):pos_type
   * off:相对位置, dir:基位置, 实际位置=dir+off
   * which: 输入流还是输出流
   * return: 移动后的绝对位置
   */
  streambuf* buf = ss.rdbuf();
  stringstream::pos_type cur = buf->pubseekoff(0, ss.cur, ios_base::out);  // 记录当前位置, 用于后面恢复
  stringstream::pos_type end = buf->pubseekoff(0, ss.end, ios_base::out);
  buf->pubseekpos(cur, ios_base::out);  // 恢复
  return (int)(uint32_t)end;
}


// 当前时间戳
inline uint64_t nowSec() {
  return std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}
inline uint64_t nowUs() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}


// duration
inline std::chrono::duration<int, std::ratio<1LL, 1LL>> buildDurationSec(int sec) {
  return std::chrono::duration<int, std::ratio<1LL, 1LL>>(sec);
}
inline std::chrono::duration<int, std::milli> buildDurationMs(int ms) {
  return std::chrono::duration<int, std::milli>(ms);
}
inline std::chrono::duration<int, std::micro> buildDurationUs(int us) {
  return std::chrono::duration<int, std::micro>(us);
}


// sleep
inline void sleepSec(int sec) {
  std::this_thread::sleep_for(buildDurationSec(sec));
}
inline void sleepMs(int ms) {
  std::this_thread::sleep_for(buildDurationMs(ms));
}
inline void sleepUs(int us) {
  std::this_thread::sleep_for(buildDurationUs(us));
}


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

template<typename T>
shared_ptr<T> createObj(string&& s) {
  return make_shared<T>(s);
}

#define DEF_SHARED_PTR(CLZ) typedef std::shared_ptr<CLZ> CLZ##Ptr;

struct Aoo;
struct Boo;
struct Coo;
DEF_SHARED_PTR(Aoo);
DEF_SHARED_PTR(Boo);
DEF_SHARED_PTR(Coo);

struct Aoo {
  Aoo(string s="") : id(++id_), v(s) { cout << "Aoo-con-" << id << "-" << v << endl; }
  Aoo(const Aoo& x) : id(++id_), v(x.v) { cout << "Aoo-con&-" << id << "-" << v << endl; }
  Aoo(const Aoo&& x) : id(++id_), v(x.v) { cout << "Aoo-con&&-" << id << "-" << v << endl; }
  ~Aoo() { cout << "Aoo-decon-" << id << "-" << v << endl; }
  uint32_t id;
  string v;
  BooPtr pb;
  static atomic<uint32_t> id_;
};

struct Boo {
  Boo(string s="") : id(++id_), v(s) { cout << "Boo-con-" << id << "-" << v << endl; }
  Boo(const Boo& x) : id(++id_), v(x.v) { cout << "Boo-con&-" << id << "-" << v << endl; }
  Boo(const Boo&& x) : id(++id_), v(x.v) { cout << "Boo-con&&-" << id << "-" << v << endl; }
  ~Boo() { cout << "Boo-decon-" << id << "-" << v << endl; }
  uint32_t id;
  string v;
  AooPtr pa;
  static atomic<uint32_t> id_;
};

struct Coo {
  Coo(string s="") : id(++id_), v(s) { cout << "Coo-con-" << id << "-" << v << endl; }
  Coo(const Coo& x) : id(++id_), v(x.v) { cout << "Coo-con&-" << id << "-" << v << endl; }
  Coo(const Coo&& x) : id(++id_), v(x.v) { cout << "Coo-con&&-" << id << "-" << v << endl; }
  ~Coo() { cout << "Coo-decon-" << id << "-" << v << endl; }
  uint32_t id;
  string v;
  AooPtr pa;
  BooPtr pb;
  static atomic<uint32_t> id_;
};

class TestStatic {
public:
  static const set<string> empty_set;
};
