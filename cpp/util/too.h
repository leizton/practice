#include "util/cpp_lib.h"

struct TrivialInt {
  int v;
};

struct Int {
  int v;
  Int(int v_ = 0) : v(v_) {}
  reload_os_out(Int) { out << obj.v; return out; }
  friend bool operator<(const Int& x, const Int& y) { return x.v < y.v; }
};

struct Aoo;
struct Boo;
struct Coo;
typedef shared_ptr<Aoo> AooPtr;
typedef shared_ptr<Boo> BooPtr;
typedef shared_ptr<Coo> CooPtr;

struct Aoo {
  Aoo(string s="") : id(++id_), v(s) { cout << "Aoo-con-" << id << "-" << v << endl; }
  Aoo(const Aoo& x) : id(++id_), v(x.v) { cout << "Aoo-con&-" << id << "-" << v << endl; }
  Aoo(const Aoo&& x) : id(++id_), v(x.v) { cout << "Aoo-con&&-" << id << "-" << v << endl; }
  ~Aoo() { cout << "Aoo-decon-" << id << "-" << v << endl; }
  const uint32_t id;
  string v;
  BooPtr pb;
private:
  static atomic<uint32_t> id_;
};

struct Boo {
  Boo(string s="") : id(++id_), v(s) { cout << "Boo-con-" << id << "-" << v << endl; }
  Boo(const Boo& x) : id(++id_), v(x.v) { cout << "Boo-con&-" << id << "-" << v << endl; }
  Boo(const Boo&& x) : id(++id_), v(x.v) { cout << "Boo-con&&-" << id << "-" << v << endl; }
  ~Boo() { cout << "Boo-decon-" << id << "-" << v << endl; }
  const uint32_t id;
  string v;
  AooPtr pa;
private:
  static atomic<uint32_t> id_;
};

struct Coo {
  Coo(string s="") : id(++id_), v(s) { cout << "Coo-con-" << id << "-" << v << endl; }
  Coo(const Coo& x) : id(++id_), v(x.v) { cout << "Coo-con&-" << id << "-" << v << endl; }
  Coo(const Coo&& x) : id(++id_), v(x.v) { cout << "Coo-con&&-" << id << "-" << v << endl; }
  ~Coo() { cout << "Coo-decon-" << id << "-" << v << endl; }
  const uint32_t id;
  string v;
  AooPtr pa;
  BooPtr pb;
private:
  static atomic<uint32_t> id_;
};