#include <iostream>
#include <memory>

using namespace std;

// 管理资源. 利用析构函数释放资源
void manageRes() {
  int v = 0;
  shared_ptr<void> ptr(nullptr, [&v](void* p) {
    cout << "release v=" << v << endl;
  });

  v += 100;
}

int main() {
  manageRes();
}