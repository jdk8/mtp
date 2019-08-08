#include "actor.h"
#include "helper.h"
#include "worker.h"
#include "reflect.h"
#include <sstream>
#include <queue>
#include "messager.h"
#include "logger.h"
#include "server_actor.h"
#include "actor_server.h"

using namespace mtp;

void ServerActor::CallClient(const std::string& callee_str, const std::string& method, const Message& message) {
  Mail callee(callee_str);
  CallClient(callee, method, message);
}

void ServerActor::CallServer(const std::string& callee_str, const std::string& method, const Message& message) {
  Mail callee(callee_str);
  CallServer(callee, method, message);
}

void ServerActor::CallClient(const Mail& callee, const std::string& method, const Message& message) {
  RemoteCallMessage* m = worker()->remote_call_pool().GetObject();
  m->Reset(callee.SerializeAsString(), kActorMessageTypeCallClient, method, message.SerializeAsString()
    , callee.conn_fd());
  m->set_allocator_id(worker()->worker_id());
  Messager::Instance().ServerRemoteCall(*m);
}

void ServerActor::CallServer(const Mail& callee, const std::string& method, const Message& message) {
  LocalCallMessage* m = worker()->local_call_pool().GetObject();
  m->Reset(callee.actor_id(), method, message.SerializeAsString());
  m->set_allocator_id(worker()->worker_id());
  Messager::Instance().WorkerLocalCall(callee.worker_id(), *m);
}

void ServerActor::AddCallback(int timeout, Closure* closure) {
  worker()->AddCallback(timeout, closure);
}

Actor* ServerActor::NewLocalActor(const std::string& actor_name) {
  Actor* actor = NewActor(actor_name);
  std::string actor_id = UUIDHelper::GenId();
  actor->set_mail(new Mail(ActorServer::Instance().ip(), ActorServer::Instance().port(),
    actor_id, worker()->worker_id()));
  worker()->InitActor(*actor);
  return actor;
}

void ClientService::ClientJoin(const Mail& mail) {
  OnClientJoin(mail);
}

void ClientService::ClientLeave(const Mail& mail) {
  OnClientLeave(mail);
}

void ServerActor::CallRandService(const std::string& actor_name, const std::string& method, const Message& message) {
  Mail* mail = ActorServer::Instance().RandServiceMail(actor_name);
  if (mail) {
    CallServer(*mail, method, message);
  }
}

void ServerActor::CallShardService(const std::string& actor_name, std::string shard_key,
  const std::string& method, const Message& message) {
  Mail* mail = ActorServer::Instance().ShardServiceMail(actor_name, shard_key);
  if (mail) {
    CallServer(*mail, method, message);
  }
}

void ServerActor::CallShardService(const std::string& actor_name, uint32_t shard_no,
  const std::string& method, const Message& message) {
  Mail* mail = ActorServer::Instance().ShardServiceMail(actor_name, shard_no);
  if (mail) {
    CallServer(*mail, method, message);
  } else {
    LOG_ERROR << "[ServerActor::CallShardService] Service [" << actor_name << "," << shard_no << "] doesn't exist." << method;
  }
}