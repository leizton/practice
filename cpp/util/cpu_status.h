#pragma once

#include <bits/stdc++.h>

/**
 * 采样周期至少是2秒, 否则不准
 */
class CpuStatus {
public:
  CpuStatus() {
    prev_total_ = prev_idle_ = 0;
    getCpuStatus();
  }

  int getCpuStatus() {
    std::ifstream proc_stat_in("/proc/stat");
    std::string line, cpu_name;
    std::getline(proc_stat, line);
    std::istringstream line_in(line);
    line_in >> cpu_name;
    int64_t v = 0, total = 0, idle = 0;
    for (int i = 0; line_in >> v; i++) {
      total += v;
      if (i == 3) idle = v;
    }

    int64_t total_delta = std::max<int64_t>(1, total - prev_total_);
    int64_t idle_delta = std::max<int64_t>(1, idle - prev_idle_);
    prev_total_ = total;
    prev_idle_ = idle;

    int64_t res = 100 * (total_delta - idle_delta) / total_delta;
    return std::min(100, static_cast<int>(res));
  }

private:
  int64_t prev_total_ = 0;
  int64_t prev_idle_ = 0;
};
