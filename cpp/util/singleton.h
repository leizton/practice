#include <memory>

template <class T>
class Singleton {
public:
  static T& instance() {
    static const std::unique_ptr<T> instance_{new T}; // 进程退出时会析构
    return *instance_;
  }

protected:
  Singleton() {}
  virtual ~Singleton() {}
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
};

#ifdef EXAMPLE
// test.h
class Test : public Singleton<Test> {
public:
  ~Test() {}
};
// main.cc
Test& t = Test::instance();
#endif

// -----------------------------------------------------------------------------------------------------------
#ifdef GOOGLE_SINGLETON
template<class T>
class Singleton {
private:
  class InstanceHolder {
  public:
    std::unique_ptr<T> instance_;

    InstanceHolder() {
      instance_.reset(new T);
    }

    ~InstanceHolder() {  // 程序退出时析构InstanceHolder，进而析构T
    }
  };
  static InstanceHolder holder_;

public:
  static T& instance() {  // 引用静态的holder_
    return *holder_.instance_;
  }

private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);
};

// 因为是模板，所以static成员变量的定义写在.h而不是.cc里
template<class T>
typename Singleton<T>::InstanceHolder Singleton<T>::holder_;
#endif

#ifdef EXAMPLE
// test.h
class Test {
public:
  ~Test() {}
};
typedef Singleton<Test> TestSingleton;
// main.cc
Test& t = TestSingleton::instance();
#endif
