#ifndef TCP_CLIENT_H__
#define TCP_CLIENT_H__

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <google/protobuf/service.h>
#include "epoller.h"
#include "lt_epoller.h"
#include "et_epoller.h"

using namespace google::protobuf;

namespace mtp {

class TcpClient: public LtEpoller {
 public:
  TcpClient () {};
  ~TcpClient () {};
  void Connect (std::string ip, int port, Service* service);
  void Close ();
  void HandleClose();
  RpcChannel* get_rpc_channel() { return rpc_channel_; };
  std::string& GetWriteBuffer(int conn_fd);
  void HandleEvent(int event_fd, int event);
  void HandleClose(int conn_fd);

 private:
  char* ip_;
  int port_;
  int client_fd_;
  Service* service_;
  RpcChannel* rpc_channel_;
  std::string write_buf_;
};

}

#endif
