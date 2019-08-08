#ifndef MYRPC_CHANNEL_H__
#define MYRPC_CHANNEL_H__

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "rpc_package.pb.h"
#include "myrpc_controller.h"
#include "epoller.h"
#include "const.h"
#include <unordered_map>

using namespace google::protobuf;

namespace mtp {

struct MyRpcCallback {
  MyRpcCallback(Closure* c, Message* r) : closure(c), response(r) {};
  Closure* closure;
  Message* response;
};

class MyRpcChannel : public RpcChannel {
 public:
  MyRpcChannel(Service* service, int conn_fd, Epoller* epoller);
  ~MyRpcChannel();
  void CallMethod(const MethodDescriptor* method,
                  RpcController* controller,
                  const Message* request,
                  Message* response,
                  Closure* done);
  void ParseData(char* data, int length);

 private:
  void RunCallback(const Message* response, uint32_t callback_id);
  uint32_t SaveCallback(Closure* done, Message* response);
  void SendMessage(int index, uint32_t callback_id, const Message* message);
 
  Service* service_;
  RpcPackage send_pkg_;
  RpcPackage recv_pkg_;
  char recv_buf_[kMaxRpcSize * 4];
  char write_buf_[kMaxRpcSize * 4];
  int recv_buf_len_;
  int conn_fd_;
  uint32_t callback_id_;
  Epoller* epoller_;
  MyRpcController rpc_controller_;
  std::unordered_map<uint32_t, MyRpcCallback*> callbacks_;
};

}

#endif
