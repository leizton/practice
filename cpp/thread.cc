#include "common/header.h"

const int kRunnableThreadNum = thread::hardware_concurrency();

thread_local char gtlStr[64];
thread_local int gtlCount = 0;

inline uint64_t timeDiff(const chrono::time_point<std::chrono::system_clock>& start,
                         const chrono::time_point<std::chrono::system_clock>& end) {
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    mutex mtx;
    condition_variable cond;
    bool ready = false;
    auto start = chrono::system_clock::now();
    atomic<int> thread_count(0);

    vector<thread> threads;
    for (int i = 0; i < kRunnableThreadNum; ++i) {
        threads.push_back(thread([&]() {
            const int threadId = ++thread_count;
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&ready]() { return ready; });
            auto tp = chrono::system_clock::now();
            snprintf(gtlStr, 64, "th %d: %llu %p %d", threadId, timeDiff(start, tp), gtlStr, ++gtlCount);
            printf("%s\n", gtlStr);
            --thread_count;
        }));
    }

    this_thread::sleep_for(chrono::seconds(1));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // 8
    auto tp = chrono::system_clock::now();
    {
        unique_lock<mutex> lock(mtx);
        ready = true;
        cond.notify_all();
    }

    for (auto& th : threads) {
        th.join();
    }
    printf("time: %llu\n", timeDiff(start, tp));
    printf("thread_count: %d\n", thread_count.load(memory_order_relaxed));  // 8
    return 0;
}