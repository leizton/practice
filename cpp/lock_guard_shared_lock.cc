#include <iostream>
#include <shared_mutex>

using namespace std;

class RwLock {
public:
  void lock_shared() {
    cout << "lock_shared" << endl;
  }
  void unlock_shared() {
    cout << "unlock_shared" << endl;
  }
  void lock() {
    cout << "lock" << endl;
  }
  void unlock() {
    cout << "unlock" << endl;
  }
};

int main() {
  RwLock mtx;
  {
    lock_guard<RwLock> lock(mtx);
  }
  {
    shared_lock<RwLock> lock(mtx);
  }
}