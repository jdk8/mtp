#ifndef ACTOR_CLIENT_H__
#define ACTOR_CLIENT_H__

#include "helper.h"
#include "actor.pb.h"
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <map>
#include "reflect.h"
#include "actor.h"
#include "worker.h"
#include "tcp_client.h"
#include <sstream>
#include "messager.h"

using namespace google::protobuf;

namespace mtp {

class ActorClientServiceImpl : public ActorClientService {
 public:
  void SendActorMessage(google::protobuf::RpcController* controller, const ActorMessage* request,
    ActorMessage* response, google::protobuf::Closure* done);

 private:
  InitClient init_client_;
};

class ActorClient {
 public:
  static ActorClient& Instance() {
    static ActorClient actor_client;
    return actor_client;
  }

  void Init(const std::string& ip, int port, const std::string& actor_dir);
  void InitClientActor(int conn_fd);
  void RunForever();

  ActorService* server_stub() { return server_stub_; }
  void set_client_service_mail(Mail* mail) { client_service_mail_ = mail; }
  Mail* client_service_mail() { return client_service_mail_; }
  Actor* client_actor() { return client_actor_; }

 private:
  std::string ip_;
  int port_;
  std::string actor_dir_;
  ActorClientServiceImpl actor_client_service_impl;
  TcpClient tcp_client_;
  ActorService* server_stub_;
  Mail* client_service_mail_;
  Actor* client_actor_;
};

}

#endif