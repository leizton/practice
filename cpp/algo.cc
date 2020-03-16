#include "chrono_util.h"
#include "base.h"
#include "gre_words.h"


// #define RUN lowerBoundAndUpperBound
void lowerBoundAndUpperBound() {
  vector<int> a{1,2,2,4,5};
  auto lower = [&a] (int target) {
    return std::lower_bound(a.begin(), a.end(), target) - a.begin();
  };
  auto upper = [&a] (int target) {
    return std::upper_bound(a.begin(), a.end(), target) - a.begin();
  };

  // lower_bound() 返回target比多少个元素大(不包括等于)
  COUT("lower");
  COUT(lower(-1));  // 0
  COUT(lower(2));   // 1
  COUT(upper(3));   // 3
  COUT(lower(6));   // 5

  // upper_bound() 返回target比多少个元素大(包括等于)
  COUT("\nupper");
  COUT(upper(-1));  // 0
  COUT(upper(2));   // 3
  COUT(upper(3));   // 3
  COUT(upper(6));   // 5
}


atomic<uint32_t> Aoo::id_;
atomic<uint32_t> Boo::id_;
atomic<uint32_t> Coo::id_;
const set<string> TestStatic::empty_set;


int main() {
  std::cout << std::boolalpha;
  std::cout << "----------" << std::endl;
  RUN();
  std::cout << "----------" << std::endl;
}