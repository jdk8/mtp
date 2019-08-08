#ifndef ACTOR_H__
#define ACTOR_H__

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
#include "logger.h"

using namespace google::protobuf;

namespace mtp {

class Worker;

enum ActorMessageType {
  kActorMessageTypeInitClient = 0,
  kActorMessageTypeCallServer,
  kActorMessageTypeCallClient,
  kActorMessageTypeClientJoin
};

class Actor {
 public:
  Actor() : mail_(NULL) {}
  virtual ~Actor() {
    if (mail_ != NULL) {
      delete mail_;
    }
  };

  virtual void Init() {};
  virtual void CallClient(const Mail& callee, const std::string& method, const Message& message) = 0;
  virtual void CallServer(const Mail& callee, const std::string& method, const Message& message) = 0;
  virtual void CallClient(const std::string& callee_str, const std::string& method, const Message& message) = 0;
  virtual void CallServer(const std::string& callee_str, const std::string& method, const Message& message) = 0;
  virtual void AddCallback(int timeout, Closure* closure) = 0;

  void set_mail(Mail* mail) { mail_ = mail; }
  Mail* mail() { return mail_; }
  void set_worker(Worker* worker) { worker_ = worker; }
  Worker* worker() { return worker_; }
  std::string actor_id() { return mail_->actor_id(); }
  const std::string& worker_id() const { return mail_->worker_id(); }
  void set_timer(Timer* timer) { timer_ = timer; }
  Timer* timer() { return timer_; }

 private:
  std::string actor_id_;
  Mail* mail_;
  Worker* worker_;
  Timer* timer_;
};

}

#endif