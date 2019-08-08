#include "tcp_server.h"
#include "helper.h"
#include "actor.pb.h"
#include "actor.h"
#include "messager.h"
#include "logger.h"
#include "actor_server.h"
#include "const.h"

using namespace google::protobuf;
using namespace mtp;

TcpServer::~TcpServer() {
  for(auto& kv : free_objects_) {
    delete kv.second;
  }
}

void TcpServer::ExportOnPort (const char* port, Service* service) {
  service_ = service;
  port_ = port;
  listen_fd_ = SocketHelper::OpenListenfd(std::atoi(port));
  SocketHelper::SetSocketNonBlock(listen_fd_);
  InitEpollSocket(listen_fd_);
}

void TcpServer::HandleEvent(int event_fd, int event) {
  if ((event & EPOLLERR) || (event & EPOLLHUP)) {
    LOG_ERROR << "Epoll error";
    close (event_fd);
  } else if (event_fd == listen_fd_) {
    int conn_fd;
    if ((conn_fd = SocketHelper::Accept(listen_fd_)) > 0) {
      HandleAccept(conn_fd);
      SetSocketNonBlock(conn_fd);
      InitEpollSocket(conn_fd);
    }
  } else if (event_fd != listen_fd_) {
    if (event & EPOLLIN) {
      std::string readed_buf;
      HandleRead(event_fd, readed_buf);
      if (readed_buf.length() > 0) {
        auto iter = rpc_channels_.find(event_fd);
        if (iter != rpc_channels_.end()) {
          iter->second->ParseData(const_cast<char*>(readed_buf.c_str()), readed_buf.length());
        } else {
          LOG_ERROR << "Event fd not found:" << event_fd;
        }
      }
    } else if (event & EPOLLOUT) {
      HandleWrite(event_fd, write_bufs_[event_fd]);
    }
  }
}

std::string& TcpServer::GetWriteBuffer(int conn_fd) {
  return write_bufs_[conn_fd];
}

void TcpServer::HandleAccept(int conn_fd) {
  rpc_channels_[conn_fd] = new MyRpcChannel(service_, conn_fd, this);
  ActorClientService* stub = new ActorClientService::Stub(rpc_channels_[conn_fd]);
  ActorServer::Instance().SetClientStub(conn_fd, stub);
  ActorMessage req;
  req.set_type(kActorMessageTypeInitClient);
  InitClient init_client;
  init_client.set_conn_fd(conn_fd);
  init_client.set_client_service(ActorServer::Instance().client_service().SerializeAsString());
  req.set_message(init_client.SerializeAsString());
  stub->SendActorMessage(NULL, &req, NULL, NULL);
  write_bufs_[conn_fd] = "";
}

void TcpServer::TickMessage() {
  Messager::Instance().ConsumeServerMessage(*this);
  Messager::Instance().TransferToLockFreeQueue();
}

void TcpServer::ClientJoin(int conn_fd, const Mail& mail) {
  client_mails_[conn_fd] = &mail;
}

void TcpServer::SendMessage(int conn_fd, const ActorMessage& message) const {
  ActorClientService* stub = ActorServer::Instance().GetClientStub(conn_fd);
  if (stub) {
    stub->SendActorMessage(NULL, &message, NULL, NULL);
  }
}

void TcpServer::HandleClose(int conn_fd) {
  auto mail_iter = client_mails_.find(conn_fd);
  if (mail_iter != client_mails_.end()) {
    const Mail& client_mail = *mail_iter->second;
    Messager::Instance().WorkerClientLeave(client_mail);
    delete mail_iter->second;
    client_mails_.erase(mail_iter);
  }
  auto channel_iter = rpc_channels_.find(conn_fd);
  if (channel_iter != rpc_channels_.end()) {
    delete channel_iter->second;
    rpc_channels_.erase(channel_iter);
  }
}

void TcpServer::FreeObject(const std::vector<BaseMessage*>& free_objects) {
  for (auto& m : free_objects) {
    m->Release(*this);
  }
}

void TcpServer::ReturnObjectToAllocator(BaseMessage* m) {
  const std::string& allocator_id = m->allocator_id();
  auto iter = free_objects_.find(allocator_id);
  if (iter == free_objects_.end()) {
    free_objects_[allocator_id] = new std::vector<BaseMessage*>;
  }
  free_objects_[allocator_id]->push_back(m);
  if (free_objects_[allocator_id]->size() > kMaxFreeObjectReturn && allocator_id != "") {
    FreeObjectMessage* m = free_object_pool_.GetObject();
    m->Reset(*free_objects_[allocator_id]);
    m->set_allocator_id(kServerId);
    if (allocator_id == kServerId) {
      Messager::Instance().ServerFreeObject(*m);
    } else {
      Messager::Instance().WorkerFreeObject(allocator_id, *m);
    }
    free_objects_[allocator_id]->clear();
  }
}
