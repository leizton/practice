#include "common/header.h"

const int kRunnableThreadNum = thread::hardware_concurrency();

const int kGtlStrSize = 64;
thread_local char gtlStr[kGtlStrSize];
thread_local int gtlCount = 0;

inline uint64_t timeDiff(const chrono::time_point<std::chrono::system_clock>& end,
                         const chrono::time_point<std::chrono::system_clock>& start) {
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    mutex mtx;
    condition_variable cond;
    bool ready = false;
    auto start_time = chrono::system_clock::now();
    atomic<int> thread_count(0);

    printf("thread num: %d\n", kRunnableThreadNum);
    vector<thread> threads;
    for (int i = 0; i < kRunnableThreadNum; ++i) {
        threads.push_back(thread([&]() {
            const int threadId = ++thread_count;
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&ready]() { return ready; });
            auto time_point = chrono::system_clock::now();
            snprintf(gtlStr, kGtlStrSize, "th %d: %lu %p %d", threadId, timeDiff(time_point, start_time), gtlStr, ++gtlCount);
            printf("%s\n", gtlStr);
            --thread_count;
        }));
    }

    this_thread::sleep_for(chrono::seconds(1));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // kRunnableThreadNum
    auto time_point = chrono::system_clock::now();
    {
        unique_lock<mutex> lock(mtx);
        ready = true;
        cond.notify_all();
    }

    for (auto& th : threads) {
        th.join();
    }
    printf("time: %lu\n", timeDiff(time_point, start_time));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // 0
    return 0;
}