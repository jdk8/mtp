#include "myrpc_channel.h"
#include "myrpc_controller.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <limits.h>

using namespace google::protobuf;
using namespace mtp;

MyRpcChannel::MyRpcChannel(Service* service, int conn_fd, Epoller* epoller)
  : service_(service),
  recv_buf_len_(0),
  conn_fd_(conn_fd),
  callback_id_(1),
  epoller_(epoller),
  rpc_controller_(conn_fd){
}

MyRpcChannel::~MyRpcChannel() {
  for (auto& kv : callbacks_) {
    delete kv.second;
  }
}

uint32_t MyRpcChannel::SaveCallback(Closure* done, Message* response) {
  if (std::numeric_limits<uint32_t>::max() == callback_id_) {
    callback_id_ = 1;
  }
  callbacks_[callback_id_] = new MyRpcCallback(done, response);
  return callback_id_++;
}

void MyRpcChannel::SendMessage(int index, uint32_t callback_id, const Message* message) {
  send_pkg_.Clear();
  send_pkg_.set_index(index);
  if (callback_id > 0) {
    send_pkg_.set_callback_id(callback_id);
  }
  int m_size = message->ByteSize();
  if (m_size > kMaxRpcSize) {
    return;
  }
  message->SerializeToArray(write_buf_, m_size);
  send_pkg_.set_data(write_buf_, m_size);
  int pkg_size = send_pkg_.ByteSize();
  send_pkg_.SerializeToArray(write_buf_ + 4, pkg_size);
  char* int_buf = reinterpret_cast<char*>(&pkg_size);
  memcpy(write_buf_, int_buf, 4);
  std::string& write_buf = epoller_->GetWriteBuffer(conn_fd_);
  write_buf.append(write_buf_, pkg_size + 4);
  epoller_->HandleWrite(conn_fd_, write_buf);
}

void MyRpcChannel::RunCallback(const Message* response, uint32_t callback_id) {
  SendMessage(-1, callback_id, response);
}

void MyRpcChannel::CallMethod(const MethodDescriptor* method,
                              RpcController* controller,
                              const Message* request,
                              Message* response,
                              Closure* done) {
  int callback_id = 0;
  if (done) {
    callback_id = SaveCallback(done, response);
  }
  SendMessage(method->index(), callback_id, request);
}

void MyRpcChannel::ParseData(char* data, int length){
  if (recv_buf_len_ == 0 && length >= 4) { // New rpc pack
    char int_buf[4];
    memcpy(int_buf, data, 4);
    int pkg_size = *reinterpret_cast<int*>(int_buf);
    if (pkg_size > kMaxRpcSize) {
      return;
    }
    if (length - 4 < pkg_size) {
      memcpy(recv_buf_, data, length);
      recv_buf_len_ = length;
    } else {
      recv_pkg_.ParseFromArray(data + 4, pkg_size);
      if (recv_pkg_.index() < 0) {
        MyRpcCallback* rpc_callback = callbacks_[recv_pkg_.callback_id()];
        rpc_callback->response->ParseFromArray(recv_pkg_.data().c_str(), recv_pkg_.data().size());
        rpc_callback->closure->Run();
      } else {
        const MethodDescriptor* method = service_->GetDescriptor()->method(recv_pkg_.index());
        Message* request = const_cast<Message*>(&service_->GetRequestPrototype(method));
        request->ParseFromArray(recv_pkg_.data().c_str(), recv_pkg_.data().size());
        if (recv_pkg_.callback_id() > 0) {
          const Message* response = &service_->GetResponsePrototype(method);
          Closure* closure = const_cast<Closure*>(NewCallback(this, &MyRpcChannel::RunCallback, response, recv_pkg_.callback_id()));
          service_->CallMethod(method, &rpc_controller_, request, const_cast<Message*>(response), closure);
        } else {
          service_->CallMethod(method, &rpc_controller_, request, NULL, NULL);
        }
      }
      if (length - 4 > pkg_size) {
        ParseData(data + pkg_size + 4, length - 4 - pkg_size);
      }
    }
  } else if (recv_buf_len_ == 0 && length < 4) {
    memcpy(recv_buf_, data, length);
    recv_buf_len_ = length;
  } else {
    memcpy(recv_buf_ + recv_buf_len_, data, length); // May overflow
    int total_len = recv_buf_len_ + length;
    recv_buf_len_ = 0;
    ParseData(recv_buf_, total_len);
  }
}
