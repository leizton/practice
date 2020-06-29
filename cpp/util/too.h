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
public:
  Aoo()
    : id(++id_), v(std::to_string(id))
    { if(log) cout << "Aoo-con-" << id << endl; con_num_++; }

  Aoo(string s)
    : id(++id_), v(s)
    { if(log) cout << "Aoo-con-" << id << endl; con_num_++; }

  ~Aoo()
    { if(log) cout << "Aoo-decon-" << id << endl; decon_num_++; }

  Aoo(const Aoo& x)
    : id(++id_), v(x.v)
    { if(log) cout << "Aoo-con&-" << id << endl; copy_con_num_++; }

  Aoo(const Aoo&& x)
    : id(++id_), v(x.v)
    { if(log) cout << "Aoo-con&&-" << id << endl; move_copy_con_num_++; }

  const uint32_t id;
  string v;
  BooPtr pb;

public:
  static int con() { return (int)con_num_.load(); }
  static int decon() { return (int)decon_num_.load(); }
  static int copy_con() { return (int)copy_con_num_.load(); }
  static int move_con() { return (int)move_copy_con_num_.load(); }
  static void reset() { id_ = con_num_ = decon_num_ = copy_con_num_ = move_copy_con_num_ = 0; }

  static atomic<bool> log;

private:
  static atomic<uint32_t> id_;
  static atomic<uint32_t> con_num_;
  static atomic<uint32_t> decon_num_;
  static atomic<uint32_t> copy_con_num_;
  static atomic<uint32_t> move_copy_con_num_;
};

struct Boo {
  Boo(string s="") : id(++id_), v(s) { cout << "Boo-con-" << id << "-" << v << endl; }
  Boo(const Boo& x) : id(++id_), v(x.v) { cout << "Boo-con&-" << id << "-" << v << endl; }
  Boo(const Boo&& x) : id(++id_), v(x.v) { cout << "Boo-con&&-" << id << "-" << v << endl; }
  ~Boo() { cout << "Boo-decon-" << id << "-" << v << endl; }
  const uint32_t id;
  string v;
  AooPtr pa;
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
  static atomic<uint32_t> id_;
};

struct TestA {
  int a;
  TestA(int _a = 0) : a(_a) { print("TestA-con-" + to_string(a)); }
  virtual ~TestA() { print("TestA-decon-" + to_string(a)); }
};

struct TestB : public TestA {
  int b;
  TestB(int _b = 0, int _a = 0) : TestA(_a), b(_b) {
    print("TestB-con-" + to_string(b));
  }
  virtual ~TestB() { print("TestB-decon-" + to_string(b)); }
};
