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

    vector<thread> threads;
    for (int i = 0; i < kRunnableThreadNum; ++i) {
        threads.push_back(thread([&, i]() {
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&ready]() { return ready; });
            auto tp = chrono::system_clock::now();
            snprintf(gtlStr, 64, "th %d: %llu %p %d", i, timeDiff(start, tp), gtlStr, ++gtlCount);
            printf("%s\n", gtlStr);
        }));
    }

    this_thread::sleep_for(chrono::seconds(1));
    auto tp = chrono::system_clock::now();
    {
        unique_lock<mutex> lock(mtx);
        ready = true;
        cond.notify_all();
    }

    for (auto& th : threads) {
        th.join();
    }
    printf("main: %llu\n", timeDiff(start, tp));
    return 0;
}