#include "QosManager.h"

bool QosManager::CreateAndRegisterRpcQos(autil::legacy::Jsonizable::JsonWrapper& json) {
  auto qos = std::make_shared<RpcQos>();
  qos->Jsonize(json);
  if (qos->Init() == false) {
    DFLOG(ERROR, "RpcQos init fail. ");
    return false;
  }
  auto iter = rpc_qos_map_.find(qos->GetQosName());
  if (iter != rpc_qos_map_.end()) {
    DFLOG(ERROR, "RpcQos [%s] already exist.", qos->GetQosName().c_str());
    return false;
  }
  rpc_qos_map_.emplace(qos->GetQosName(), qos);
  DFLOG(INFO, "RpcQos [%s] registered.", qos->GetQosName().c_str());
  return true;
}

std::shared_ptr<RpcQos> QosManager::GetRpcQos(const std::string& name) {
  auto iter = rpc_qos_map_.find(name);
  return (iter != rpc_qos_map_.end()) ? iter->second : nullptr;
}

bool QosManager::Init() {
  LOG_INFO() << "QosManager init.";
  for (auto& iter : flow_valve_map_) {
    if (iter.second->Init() == false) {
      LOG_INFO() << "RpcQos[" << iter.first << "] init fail.";
      return false;
    }
  }
  init_ = true;
  run_update_status_ = true;
  update_status_loop_thread_ = std::thread([this] () {
    UpdateStatusLoopThread();
  });
  LOG_INFO() << "QosManager init done.";
  return true;
}

void QosManager::Stop() {
  if (init_ == false) {
    return;
  }
  DFLOG(INFO, "QosManager stopping.");
  init_ = false;
  run_update_status_ = false;
  DFLOG(INFO, "QosManager wait update_status_loop stop.");
  update_status_loop_thread_.join();
  DFLOG(INFO, "QosManager update_status_loop stopped.");
}

void QosManager::UpdateStatusLoopThread() {
  DFLOG(INFO, "QosManager update_status_loop begin.");
  const int interval_ms = 200;
  int64_t update_ts = tgh::nowMs();  // 下次update时间
  update_ts = update_ts - (update_ts % 1000) + 1000;  // 对齐1秒
  int log_count = 0;
  while (run_update_status_.load()) {
    update_ts += interval_ms;
    int64_t sleep_ms = update_ts - tgh::nowMs();
    if (sleep_ms > 0) tgh::sleepMs(sleep_ms);
    for (auto& iter : flow_valve_map_) {
      auto& valve = *(iter.second);
      if (valve.NeedUpdateStatus(update_ts)) {
        valve.UpdateStatus();
      }
    }
    if (++log_count == 300) {
      log_count = 0;
      DFLOG(INFO, "QosManager update_status_loop.");
    }
  }
  DFLOG(INFO, "QosManager update_status_loop end.");
}
