#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <atomic>
#include "actor.pb.h"
#include "mail.h"
#include "logger.h"
#include "actor.h"
#include "worker.h"
#include "tcp_server.h"
#include "message_type.h"

using namespace google::protobuf;
using namespace mtp;

void InitActorMessage::Process(Worker& worker) {
  worker.InitActor(*actor_);
}

void InitActorMessage::Release(TcpServer& server) {
  server.init_actor_pool().ReleaseObject(this);
  if (kDebug) {
    server.init_actor_pool().Debug(kServerId + "init_actor_pool");
  }
}

void LocalCallMessage::Process(Worker& worker) {
  worker.CallLocalActor(actor_id_, method_, message_);
}

void LocalCallMessage::Release(Worker& worker) {
  worker.local_call_pool().ReleaseObject(this);
  if (kDebug) {
    worker.local_call_pool().Debug(worker.worker_id() + "local_call_pool");
  }
}

void LocalCallMessage::Release(TcpServer& server) {
  server.local_call_pool().ReleaseObject(this);
  if (kDebug) {
    server.local_call_pool().Debug(server.server_id() + "local_call_pool");
  }
}

void ClientLeaveMessage::Process(Worker& worker) {
  worker.ClientLeave(*client_mail_);
}

void ClientLeaveMessage::Release(TcpServer& server) {
  server.client_leave_pool().ReleaseObject(this);
  if (kDebug) {
    server.client_leave_pool().Debug(server.server_id() + "client_leave_pool");
  }
}

void RemoteCallMessage::Process(TcpServer& server) {
  server.SendMessage(conn_fd_, actor_message_);
}

void RemoteCallMessage::Release(Worker& worker) {
  worker.remote_call_pool().ReleaseObject(this);
  if (kDebug) {
    worker.remote_call_pool().Debug(worker.worker_id() + "remote_call_pool");
  }
}

void ClientJoinMessage::Process(TcpServer& server) {
  server.ClientJoin(client_mail_->conn_fd(), *client_mail_);
}

void ClientJoinMessage::Process(Worker& worker) {
  worker.ClientJoin(*client_mail_);
}

void ClientJoinMessage::Release(TcpServer& server) {
  server.client_join_pool().ReleaseObject(this);
  if (kDebug) {
    server.client_join_pool().Debug(server.server_id() + "client_join_pool");
  }
}

void FreeObjectMessage::Process(TcpServer& server) {
  server.FreeObject(free_objects_);
}

void FreeObjectMessage::Process(Worker& worker) {
  worker.FreeObject(free_objects_);
}

void FreeObjectMessage::Release(Worker& worker) {
  worker.free_object_pool().ReleaseObject(this);
  if (kDebug) {
    worker.free_object_pool().Debug(worker.worker_id() + "free_object_pool");
  }
}

void FreeObjectMessage::Release(TcpServer& server) {
  server.free_object_pool().ReleaseObject(this);
  if (kDebug) {
    server.free_object_pool().Debug(server.server_id() + "free_object_pool");
  }
}