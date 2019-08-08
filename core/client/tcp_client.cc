#include "tcp_client.h"
#include "myrpc_channel.h"
#include "helper.h"
#include "messager.h"

using namespace google::protobuf;
using namespace mtp;

void TcpClient::Connect(std::string ip, int port, Service* service) {
  ip_ = const_cast<char*>(ip.c_str());
  port_ = port;
  service_ = service;
  client_fd_ = SocketHelper::OpenClienfd(ip_, port_);
  SocketHelper::SetSocketNonBlock(client_fd_);
  InitEpollSocket(client_fd_);
  rpc_channel_ = new MyRpcChannel(service_, client_fd_, this);
}

void TcpClient::Close () {
  close (client_fd_);
}

void TcpClient::HandleClose(int conn_fd) {
}

void TcpClient::HandleEvent(int event_fd, int event) {
  if ((event & EPOLLERR) || (event & EPOLLHUP)) {
    LOG_ERROR << "Epoll error";
    close (event_fd);
  } else if (event & EPOLLIN) {
    std::string readed_buf;
    HandleRead(event_fd, readed_buf);
    static_cast<MyRpcChannel*>(rpc_channel_)->ParseData(const_cast<char*>(readed_buf.c_str()), readed_buf.length());
  } else if (event & EPOLLOUT) {
    HandleWrite(event_fd, write_buf_);
  }
}

std::string& TcpClient::GetWriteBuffer(int conn_fd) {
  return write_buf_;
}