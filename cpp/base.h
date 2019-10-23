#pragma once

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

using namespace std;


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
  Aoo(string s) : v(s) { cout << "Aoo-con-" << v << endl; }
  ~Aoo() { cout << "Aoo-decon-" << v << endl; }
  string v;
  BooPtr pb;
};

struct Boo {
  Boo(string s) : v(s) { cout << "Boo-con-" << v << endl; }
  ~Boo() { cout << "Boo-decon-" << v << endl; }
  string v;
  AooPtr pa;
};

struct Coo {
  Coo(string s) : v(s) { cout << "Coo-con-" << v << endl; }
  ~Coo() { cout << "Coo-decon-" << v << endl; }
  string v;
  AooPtr pa;
  BooPtr pb;
};
