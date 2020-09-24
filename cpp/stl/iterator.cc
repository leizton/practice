#include "util/base.h"

#if comment(iterator)
  stl_iterator_base_types.h struct random_access_iterator_tag : public bidirectional_iterator_tag {};
  struct iterator<iterator_category, value_type, difference_type, pointer = value_type*, reference = value_type&>;
#endif

template <typename T>
class Container {
public:
  template <typename E>
  class Iterator : public std::iterator<std::random_access_iterator_tag, E> {
  private:
    Iterator(Container<E>* container, int pos) : container_(container), pos_(pos) {}
    friend class Container;

  public:
    E& operator*() {
      return container_->get(pos_);
    }
    bool operator==(const Iterator& it) {
      return pos_ == it.pos_;
    }
    bool operator!=(const Iterator& it) {
      return !(*this == it);
    }
    Iterator& operator++() {
      ++pos_;
      return *this;
    }
    Iterator operator++(int) {
      return Iterator(container_, pos_++);
    }
    Iterator& operator--() {
      --pos_;
      return *this;
    }
    Iterator operator--(int) {
      return Iterator(container_, pos_--);
    }
    Iterator& operator+=(int n) {
      pos_ += n;
      return *this;
    }
    Iterator operator+(int n) {
      return Iterator(container_, pos_+n);
    }
    Iterator& operator-=(int n) {
      pos_ -= n;
      return *this;
    }
    Iterator operator-(int n) {
      return Iterator(container_, pos_-n);
    }
    int operator-(const Iterator& it) {
      return pos_ - it.pos_;
    }
    bool operator<(const Iterator& it) {
      return pos_ < it.pos_;
    }
    bool operator<=(const Iterator& it) {
      return pos_ <= it.pos_;
    }
    bool operator>(const Iterator& it) {
      return pos_ > it.pos_;
    }
    bool operator>=(const Iterator& it) {
      return pos_ >= it.pos_;
    }

  private:
    Container<E>* container_;
    int pos_;
  };

  typedef Iterator<T> iterator;

  iterator begin() {
    return Iterator<T>(this, 0);
  }
  iterator end() {
    return Iterator<T>(this, (int)data_.size());
  }

  void add(int v) {
    data_.emplace_back(v);
  }
  T& get(size_t pos) {
    return data_[pos];
  }

private:
  vector<T> data_;
};

run(custom_iterator) {
  auto lst_to_str = [] (Container<Int>& lst) -> string {
    vector<int> tmp;
    for (auto& e : lst) tmp.push_back(e.v);
    return to_str(tmp);
  };

  Container<Int> lst;
  lst.add(3);
  lst.add(1);
  lst.add(2);
  assert_eq("[3,1,2]", lst_to_str(lst));

  std::sort(lst.begin(), lst.end(), [](const Int& x, const Int& y) {
    return x.v < y.v;
  });
  assert_eq("[1,2,3]", lst_to_str(lst));
}

main_run;