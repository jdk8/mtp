#ifndef MYRPC_CONTROLLER_H__
#define MYRPC_CONTROLLER_H__

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

using namespace google::protobuf;

namespace mtp {

class MyRpcController : public RpcController {
 public:
  MyRpcController () {};
  ~MyRpcController () {};
  MyRpcController(int conn_fd) : conn_fd_(conn_fd) {};

  void Reset() {};
  bool Failed() const { return false; };
  std::string ErrorText() const { return ""; };
  void StartCancel() {};
  void SetFailed(const std::string& reason) {};
  bool IsCanceled() const { return false; };
  void NotifyOnCancel(Closure* callback) {};
  int conn_fd() { return conn_fd_; };

 private:
  int conn_fd_;
};

}

#endif
