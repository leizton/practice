#include <boost/lockfree/queue.hpp>

#include "util/base.h"

enum class TestType {
  Read,
  Write,
};

class TestTaskBase {
 public:
  TestTaskBase(int _test_num)
      : test_num(_test_num) {}

  virtual void read() = 0;
  virtual void write() = 0;

 public:
  const int test_num;
  int qps = 0;
};

template <class ThreadSafeQueue>
class TestTask : public TestTaskBase {
 public:
  TestTask(ThreadSafeQueue& que, int test_num)
      : TestTaskBase(test_num), que_(que) {}

  void read() override {
    int v = 0;
    for (int i = 0; i < test_num; i++) {
      que_.pop(v);
    }
  }

  void write() override {
    for (int i = 0; i < test_num; i++) {
      que_.push(i);
    }
  }

 private:
  ThreadSafeQueue& que_;
};

void doTest(TestType test_type, TestTaskBase* task) {
  uint64_t cost = nowMs();
  switch (test_type) {
    case TestType::Read:
      task->read();
      break;
    case TestType::Write:
      task->write();
      break;
    default:
      return;
  }
  cost = nowMs() - cost;
  task->qps = ((uint64_t)task->test_num * 1000) / cost;
}

template <class ThreadSafeQueue>
void test(ThreadSafeQueue& que, int thread_num, int test_num, TestType test_type, int& avg_qps) {
  std::vector<TestTask<ThreadSafeQueue>> tests;
  for (int i = 0; i < thread_num; i++) {
    tests.emplace_back(que, test_num);
  }

  std::vector<std::thread> ths;
  for (auto& t : tests) {
    ths.emplace_back(doTest, test_type, &t);
  }
  for (auto& th : ths) {
    th.join();
  }

  int qps = 0;
  for (auto& t : tests) {
    qps += t.qps;
  }
  avg_qps = qps / (int)tests.size();
}

template <class ThreadSafeQueue>
void testConc(ThreadSafeQueue& que, int thread_num) {
  int write_qps = 0, read_qps = 0;
  test(que, thread_num, 2000000, TestType::Write, write_qps);
  test(que, thread_num, 2000000, TestType::Read, read_qps);
  LOG() << setiosflags(ios::left)
        << "th=" << setw(4) << thread_num
        << "wqps=" << setw(10) << write_qps
        << "rqps=" << setw(10) << read_qps;
}

/*
  BlockingQueue
  th=1   wqps=11173184  rqps=10256410  
  th=2   wqps=2956756   rqps=2448508   
  th=3   wqps=1357418   rqps=963145    
  th=4   wqps=1194645   rqps=833617    

  boost::lockfree::queue
  th=1   wqps=9950248   rqps=52631578
  th=2   wqps=4866180   rqps=5943549   
  th=3   wqps=2509459   rqps=3100780   
  th=4   wqps=1595757   rqps=2107616
*/
int main() {
  LOG() << "BlockingQueue";
  BlockingQueue<int> q1;
  for (int i = 1; i <= 4; i++) {
    testConc(q1, i);
  }
  LOG() << "";

  LOG() << "boost::lockfree::queue";
  boost::lockfree::queue<int> q2(128);
  for (int i = 1; i <= 4; i++) {
    testConc(q2, i);
  }
  LOG() << "";
}