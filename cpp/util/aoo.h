#pragma once

#include "cpp_lib.h"
#include "print.h"

struct TrivialInt {
  int v;
};

struct Int {
  int v;
  Int(int v_ = 0)
      : v(v_) {}
  reload_os_out(Int) {
    out << obj.v;
    return out;
  }
  friend bool operator<(const Int& x, const Int& y) { return x.v < y.v; }
  friend bool operator>(const Int& x, const Int& y) { return x.v > y.v; }
};

#define PRINT \
  if (Aoo::log) println

struct Aoo {
public:
  virtual ~Aoo() {
    decon_num_++;
    PRINT("Aoo_dector:", toString());
  }

  Aoo()
      : id(++id_)
      , dat(0) {
    con_num_++;
    PRINT("Aoo_ctor:", toString());
  }

  Aoo(int d)
      : id(++id_)
      , dat(d) {
    con_num_++;
    PRINT("Aoo_ctor:", toString());
  }

  Aoo(const Aoo& x)
      : id(++id_)
      , dat(x.dat) {
    copy_con_num_++;
    PRINT("Aoo_copy_ctor:", x.toString(), "->", toString());
  }

  Aoo(const Aoo&& x)
      : id(++id_)
      , dat(x.dat) {
    move_copy_con_num_++;
    PRINT("Aoo_move_copy_ctor:", x.toString(), "->", toString());
  }

  void operator=(const Aoo& x) {
    assign_con_num_++;
    PRINT("Aoo_assign:", x.toString(), "->", toString());
    dat = x.dat;
  }

  void operator=(const Aoo&& x) {
    move_assign_con_num_++;
    PRINT("Aoo_move_assign:", x.toString(), "->", toString());
    dat = x.dat;
  }

  const uint32_t id;
  int dat;

  std::string toString() const {
    return std::string("(") + std::to_string(id) + "," + std::to_string(dat) + ")";
  }

  static int con() { return (int)con_num_.load(); }
  static int decon() { return (int)decon_num_.load(); }
  static int copy_con() { return (int)copy_con_num_.load(); }
  static int move_con() { return (int)move_copy_con_num_.load(); }
  static int assign() { return (int)assign_con_num_.load(); }
  static int move_assign() { return (int)move_assign_con_num_.load(); }

  static void reset() {
    id_ = con_num_ = decon_num_ = 0;
    copy_con_num_ = move_copy_con_num_ = 0;
    assign_con_num_ = move_assign_con_num_ = 0;
  }

  static atomic<bool> log;

public:
  friend std::ostream& operator<<(std::ostream& os, const Aoo& a) {
    os << a.toString();
    return os;
  }

private:
  static atomic<uint32_t> id_;
  static atomic<uint32_t> con_num_;
  static atomic<uint32_t> decon_num_;
  static atomic<uint32_t> copy_con_num_;
  static atomic<uint32_t> move_copy_con_num_;
  static atomic<uint32_t> assign_con_num_;
  static atomic<uint32_t> move_assign_con_num_;
};

typedef shared_ptr<Aoo> AooPtr;

struct DAoo : public Aoo {
  ~DAoo() { PRINT("DAoo_decon_", id); }

  DAoo()
      : id(++id_) { PRINT("DAoo_con_", id); }

  const uint32_t id;

private:
  static atomic<uint32_t> id_;
};

typedef shared_ptr<DAoo> DAooPtr;

#undef PRINT
