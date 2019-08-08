#ifndef MESSAGE_TYPE_H__
#define MESSAGE_TYPE_H__

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

using namespace google::protobuf;

namespace mtp {

class Actor;
class Worker;
class TcpServer;

class BaseMessage {
 public:
  virtual ~BaseMessage() {}
  virtual void Process(Worker& worker) {}
  virtual void Process(TcpServer& server) {}
  virtual void Release(Worker& worker) {}
  virtual void Release(TcpServer& server) {}
  void set_allocator_id(const std::string& allocator_id) { allocator_id_ = allocator_id; }
  const std::string& allocator_id() { return allocator_id_; }

 private:
  std::string allocator_id_;
};

class InitActorMessage : public BaseMessage {
 public:
  InitActorMessage(
    Actor* actor)
  : actor_(actor) {}

  InitActorMessage() {}

  void Reset(Actor* actor) {
    actor_ = actor;
  }

  void Process(Worker& worker);
  void Release(TcpServer& server);

 private:
  Actor* actor_;
};

class LocalCallMessage : public BaseMessage {
 public:
  LocalCallMessage(const std::string& actor_id, const std::string& method, const std::string& message) :
    actor_id_(actor_id), method_(method), message_(message) {}

  LocalCallMessage() {}

  void Reset(const std::string& actor_id, const std::string& method, const std::string& message) {
    actor_id_ = actor_id;
    method_ = method;
    message_ = message;
  }

  void Process(Worker& worker);
  void Release(Worker& worker);
  void Release(TcpServer& server);

 private:
  std::string actor_id_;
  std::string method_;
  std::string message_;
};

class ClientLeaveMessage : public BaseMessage {
 public:
  ClientLeaveMessage(const Mail* client_mail) : client_mail_(client_mail) {}

  ClientLeaveMessage() {}

  void Reset(const Mail* client_mail) { client_mail_ = client_mail; }

  void Process(Worker& worker);
  void Release(TcpServer& server);

 private:
  const Mail* client_mail_;
};

class RemoteCallMessage : public BaseMessage {
 public:
  RemoteCallMessage(
    std::string callee_addr,
    int type,
    std::string method,
    std::string message,
    int conn_fd)
  : conn_fd_(conn_fd) {
    actor_message_.set_callee_addr(callee_addr);
    actor_message_.set_type(type);
    actor_message_.set_method(method);
    actor_message_.set_message(message);
  }

  RemoteCallMessage() {}

  void Reset(std::string callee_addr,
    int type,
    std::string method,
    std::string message,
    int conn_fd) {
	conn_fd_ = conn_fd;
	actor_message_.set_callee_addr(callee_addr);
    actor_message_.set_type(type);
    actor_message_.set_method(method);
    actor_message_.set_message(message);
  }

  ~RemoteCallMessage() {}

  void Process(TcpServer& server);
  void Release(Worker& worker);

 private:
  int conn_fd_;
  ActorMessage actor_message_;
};

class ClientJoinMessage : public BaseMessage {
 public:
  ClientJoinMessage(const Mail* client_mail) : client_mail_(client_mail) {}
  ClientJoinMessage() {}
  void Reset(const Mail* client_mail) { client_mail_ = client_mail; }

  void Process(TcpServer& server);
  void Process(Worker& worker);
  void Release(TcpServer& server);

 private:
  const Mail* client_mail_;
};

class FreeObjectMessage : public BaseMessage {
 public:
  FreeObjectMessage(const std::vector<BaseMessage*>& free_objects) {
    for (auto& o : free_objects) {
      free_objects_.push_back(o);
    }
  }
  FreeObjectMessage() {}
  void Reset(const std::vector<BaseMessage*>& free_objects) {
    free_objects_.clear();
    for (auto& o : free_objects) {
      free_objects_.push_back(o);
    }
  }

  void Process(TcpServer& server);
  void Process(Worker& worker);
  void Release(Worker& worker);
  void Release(TcpServer& server);

 private:
  std::vector<BaseMessage*> free_objects_;
};

}

#endif
