#include <iostream>
#include <fstream>
#include <exception>

#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include <type_traits>
#include <typeinfo>

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

using namespace std;


// #define RUN testUnorderedMap
void testUnorderedMap() {
  unordered_map<int, shared_ptr<int>> m;
  m[1] = make_shared<int>(1);

  auto it = m.find(1);
  if (it != m.end()) {
    *(it->second) *= 10;
  }
  cout << *(m[1]) << endl;
}


int main() {
  std::cout << std::boolalpha;
  std::cout << "----------" << std::endl;
  RUN();
  std::cout << "----------" << std::endl;
}