#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

#define RUN iter


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


int main() {
  RUN();
}