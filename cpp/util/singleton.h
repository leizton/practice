template<class T>
class Singleton {
private:
  class InstanceHolder {
  public:
    static T* instance_;

    InstanceHolder() {
      instance_ = new T;
    }

    ~InstanceHolder() {
      if (instance_) {
        delete instance_;
        instance_ = nullptr;
      }
    }
  };
  static InstanceHolder holder_;

public:
  static T& instance() {
    return *holder_.instance_;
  }

private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);
};

template<class T>
T* Singleton<T>::InstanceHolder::instance_ = nullptr;

template<class T>
typename Singleton<T>::InstanceHolder Singleton<T>::holder_;