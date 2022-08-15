#include <util/base.h>

static uint64_t PointerValBase = 0;
uint64_t ptr2int(void* p) {
  if (p == nullptr) {
    return 0;
  }
  return reinterpret_cast<uint64_t>(p) - PointerValBase;
}

set<uint64_t> eoo_pointer_set;
set<uint64_t> eoo_val_pointer_set;

struct Eoo {
  Eoo() {
    e = new int(++id);
    //
    con_num++;
    eoo_pointer_set.insert(ptr2int(this));
    eoo_val_pointer_set.insert(ptr2int(e));
    println("con Eoo", ptr2int(this), ptr2int(e), *e);
  }
  Eoo(const Eoo& ref) {
    e = new int(*ref.e);
    //
    con_num++;
    eoo_pointer_set.insert(ptr2int(this));
    eoo_val_pointer_set.insert(ptr2int(e));
    println("con Eo&", ptr2int(this), ptr2int(e), *e);
  }
  Eoo(const Eoo&& ref) {
    e = ref.e;
    //
    eoo_pointer_set.insert(ptr2int(this));
    println("con E&&", ptr2int(this), ptr2int(e), *e);
  }
  ~Eoo() {
    int *p = e, v = *e;
    if (e) {
      delete e;
      e = nullptr;
    }
    //
    eoo_pointer_set.erase(eoo_pointer_set.find(ptr2int(this)));
    if (p) {
      dcn_num++;
      eoo_val_pointer_set.erase(eoo_val_pointer_set.find(ptr2int(p)));
      println("dcn Eoo", ptr2int(this), ptr2int(p), v);
    } else {
      println("dcn Eoo", ptr2int(this));
    }
  }
  void Reset() {
    if (e) {
      *e = 0;
    }
  }
  void Moved() {
    e = nullptr;
  }

  int* e;
  static int id;
  static int con_num;
  static int dcn_num;
};

int Eoo::id = 0;
int Eoo::con_num = 0;
int Eoo::dcn_num = 0;

template <class T>
struct ResetAllocator {
  typedef T value_type;
  T* arr_ = nullptr;
  size_t sz_ = 0;

  void _alloc(size_t n) {
    arr_ = static_cast<T*>(::operator new(n * sizeof(T)));
  }
  void _free(void* p) {
    ::operator delete(p);
  }

  T* allocate(size_t n) {
    T* old_obj = arr_;
    _alloc(n);
    println("\n===== allocate", ptr2int(arr_), n, sz_);
    T* new_obj = static_cast<T*>(arr_);
    size_t i = 0;
    for (; i < sz_; i++) {
      new (new_obj+i) T(std::move(*(old_obj+i)));
      (old_obj+i)->Moved();
    }
    for (; i < n; i++) {
      new (new_obj+i) T();
    }
    return arr_;
  }
  void deallocate(void* p, size_t n) {
    println("\n===== deallocate", ptr2int(p), "->", ptr2int(arr_), n, sz_);
    T* obj = static_cast<T*>(p);
    for (size_t i = 0; i < n; i++) {
      obj->~T();
      obj++;
    }
    _free(p);
  }

  template <class ...Args>
	void construct(T* p, Args&&... args) {
    sz_++;
    // destory()的析构改到deallocate()调用, 相应地, 构造函数移到allocate()
		// new (p) T(args...);
	}
	void destroy(T* p) {
    sz_--;
    // 实例p里包含大对象, destroy时不执行p的析构只重置, 从而实现复用
    p->Reset();
	}
};

void test() {
  std::vector<Eoo, ResetAllocator<Eoo>> a;
  println("\n********************");
  println("push 3");
  a.emplace_back();
  a.emplace_back();
  a.emplace_back();
  println("\n********************");
  println("clear");
  a.clear();
  println("\n********************");
  println("push 4");
  a.emplace_back();
  a.emplace_back();
  a.emplace_back();
  a.emplace_back();
  println("\n********************");
  println("clear");
  a.clear();
  println("\n********************");
  println("push 2");
  a.emplace_back();
  a.emplace_back();
  println("\n********************");
  println("Eoo con num:", Eoo::con_num);
  println("Eoo dcn num:", Eoo::dcn_num);
}

int main() {
  PointerValBase = ptr2int(new int64_t);
  test();
  println("\n********************");
  assert_T(eoo_pointer_set.empty());
  assert_T(eoo_val_pointer_set.empty());
}
