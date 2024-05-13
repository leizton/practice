#pragma once

#include "RpcQos.h"

namespace olympus {

class QosManager {
public:
  bool CreateAndRegisterRpcQos(autil::legacy::Jsonizable::JsonWrapper& json);
  
  std::shared_ptr<RpcQos> GetRpcQos(const std::string& name);

  bool Init();
  void Stop();
  
public:
  void UpdateStatusLoopThread();
  
private:
  std::map<std::string, std::shared_ptr<RpcQos>> rpc_qos_map_;
};

}
