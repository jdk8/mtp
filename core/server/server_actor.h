#ifndef SERVER_ACTOR_H__
#define SERVER_ACTOR_H__

#include "epoller.h"
#include "actor.pb.h"
#include "mail.h"
#include "messager.h"
#include <queue>
#include <thread>
#include <vector>
#include <chrono>
#include <map>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "actor.h"

using namespace google::protobuf;

namespace mtp {

class ActorServer;

class ServerActor : public Actor {
 public:
  virtual ~ServerActor() {}
  void CallClient(const Mail& callee, const std::string& method, const Message& message);
  void CallServer(const Mail& callee, const std::string& method, const Message& message);
  void CallClient(const std::string& callee_str, const std::string& method, const Message& message);
  void CallServer(const std::string& callee_str, const std::string& method, const Message& message);
  Actor* NewLocalActor(const std::string& actor_name);
  void AddCallback(int timeout, Closure* closure);
  void CallRandService(const std::string& actor_name, const std::string& method, const Message& message);
  void CallShardService(const std::string& actor_name, std::string shard_key, const std::string& method, const Message& message);
  void CallShardService(const std::string& actor_name, uint32_t shard_no, const std::string& method, const Message& message);
};

class ServiceActor : public ServerActor {
 public:
  void set_instance(int instance) { instance_ = instance; }
  void set_shard_no(int shard_no) { shard_no_ = shard_no; }
  int instance() { return instance_; }
  int shard_no() { return shard_no_; }

 private:
  int shard_no_;
  int instance_;
};

class ClientService : public ServiceActor {
 public:
  void ClientJoin(const Mail& mail);
  void ClientLeave(const Mail& mail);

  virtual void OnClientJoin(const Mail& mail) = 0;
  virtual void OnClientLeave(const Mail& mail) = 0;
};

}

#endif