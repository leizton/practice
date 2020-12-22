#include "util/cpp_lib.h"

template<class T>
void operator +=(vector<T>& x, vector<T> y) {
  x.insert(x.end(), y.begin(), y.end());
}

inline uint64_t currThreadId() {
  return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

inline uint64_t getThreadId(const std::thread& th) {
  return std::hash<std::thread::id>{}(th.get_id());
}

inline int lengthOfOstringstream(const ostringstream& ss) {
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

#define main_run \
atomic<uint32_t> Aoo::id_(0);\
atomic<uint32_t> Aoo::con_num_(0);\
atomic<uint32_t> Aoo::decon_num_(0);\
atomic<uint32_t> Aoo::copy_con_num_(0);\
atomic<uint32_t> Aoo::move_copy_con_num_(0);\
atomic<uint32_t> Aoo::assign_con_num_(0);\
atomic<uint32_t> Aoo::move_assign_con_num_(0);\
atomic<bool>     Aoo::log(true);\
atomic<uint32_t> DAoo::id_(0);\
int main() {\
  using namespace std;\
  cout << std::boolalpha;\
  for (int i = 0; i < 5; i++) cout << "----------";\
  cout << endl;\
  RUN();\
  for (int i = 0; i < 5; i++) cout << "----------";\
  cout << endl;\
}

#define def(func) void test_##func()
#define run(func) void RUN()

#define comment(x) 0

#define COMMENT_STUFF
#define comment_begin(x) /COMMENT_STUFF*  // 因为注释先于宏处理，所以需要借助空宏COMMENT_STUFF
#define comment_end *COMMENT_STUFF/
