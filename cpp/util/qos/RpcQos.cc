#include "RpcQos.h"

#include <cmath>

void RpcQos::Jsonize(autil::legacy::Jsonizable::JsonWrapper& json) {
  json.Jsonize("qos_name", qos_name_, "");
  json.Jsonize("slide_window_step_ms", slide_window_step_ms_, 100);
  json.Jsonize("slide_window_ms", slide_window_ms_, 1000);
  json.Jsonize("slide_window_qps_limit", slide_window_qps_limit_, 1000);
}

bool RpcQos::Init() {
  if (qos_name_.empty()) {
    DFLOG(ERROR, "qos_name is empty");
    return false;
  }

  // normal slide_window_step_ms_
  if (slide_window_step_ms_ < MinSlideWindowStepMs) {
    DFLOG(ERROR, "slide_window_step_ms is invalid: %d < %d. ",
          slide_window_step_ms_, MinSlideWindowStepMs);
    return false;
  }
  {
    int n = static_cast<int>(std::ceil(double(slide_window_step_ms_) / MinSlideWindowStepMs));
    slide_window_step_ms_ = n * MinSlideWindowStepMs;
  }

  // normal slide_window_ms_
  if (slide_window_ms_ < slide_window_step_ms_) {
    DFLOG(ERROR, "slide_window_ms is invalid: %d < %d. ",
          slide_window_ms_, slide_window_step_ms_);
    return false;
  }
  slide_window_size_ = static_cast<int>(std::ceil(double(slide_window_ms_) / slide_window_step_ms_));
  slide_window_ms_ = slide_window_size_ * slide_window_step_ms_;

  slide_window_request_num_limit_ = slide_window_qps_limit_ * slide_window_ms_ / 1000;
  slide_window_request_num_limit_ = std::max(slide_window_request_num_limit_, 1);

  // init slide_window_stats_
  slide_window_stats_.resize(slide_window_size_);

  return true;
}

bool RpcQos::CheckPass() {
  return true;
}

void RpcQos::Report(const CaptureInfo& info) {
  int64_t ts = tgh::nowMs();
}

void RpcQos::Update() {
}
