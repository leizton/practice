#include "common/header.h"
#include "common/util.h"

const int kRunnableThreadNum = thread::hardware_concurrency();

const int kGtlStrSize = 64;

// thread_local使得变量在线程(非进程)作用域内全局可见
// 如果不用thread_local, 那么同一个线程在调用不同函数或调用一个函数多次时不能共享同一个变量, 而是会变成函数作用域的局部变量
// 为了保证没有竞争, thread_local变量不能被其他线程访问到
thread_local char gtlStr[kGtlStrSize];
thread_local int gtlCount = 0;

int main() {
    mutex mtx;
    condition_variable cond;
    bool ready = false;
    auto start_time = util::now();
    atomic<int> thread_count(0);

    printf("thread num: %d\n", kRunnableThreadNum);
    vector<thread> threads;
    for (int i = 0; i < kRunnableThreadNum; ++i) {
        threads.push_back(thread([&]() {
            const int threadId = ++thread_count;
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&ready]() { return ready; });
            auto timept = util::now();
            snprintf(gtlStr, kGtlStrSize, "th %d: %lu %p %d", threadId, util::timeDiff(timept, start_time), gtlStr, ++gtlCount);
            printf("%s\n", gtlStr);
            --thread_count;
        }));
    }

    this_thread::sleep_for(chrono::seconds(1));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // kRunnableThreadNum
    auto timept = util::now();
    {
        unique_lock<mutex> lock(mtx);
        ready = true;
        cond.notify_all();
    }

    for (auto& th : threads) {
        th.join();
    }
    printf("time: %lu\n", util::timeDiff(timept, start_time));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // 0
    return 0;
}