#include "util/cpp_lib.h"

template<class T>
void operator +=(vector<T>& x, vector<T> y) {
  x.insert(x.end(), y.begin(), y.end());
}

uint64_t currThreadId() {
  return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

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

class Bufcout {
public:
  Bufcout() {}
  ostringstream out_;
  constexpr static Bufcout* endl = nullptr;
};

template <class T>
Bufcout&& operator <<(Bufcout&& out, T v) {
  out.out_ << v;
  return std::move(out);
}

Bufcout&& operator <<(Bufcout&& out, Bufcout* bc) {
  if (bc != Bufcout::endl) {
    return std::move(out);  // 必须加move, move作用是转成右值引用
  }
  cout << out.out_.str() << endl;
  return std::move(out);
}

#define main_run \
atomic<uint32_t> Aoo::id_;\
atomic<uint32_t> Boo::id_;\
atomic<uint32_t> Coo::id_;\
int main() {\
  std::cout << std::boolalpha;\
  std::cout << "----------" << std::endl;\
  RUN();\
  std::cout << "----------" << std::endl;\
}

#define def(test_func) void test_func()
#define run(test_func) void RUN()
