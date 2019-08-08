#ifndef CLIENT_ACTOR_H__
#define CLIENT_ACTOR_H__

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

class ClientActor : public Actor {
 public:
  void CallClient(const Mail& callee, const std::string& method, const Message& message);
  void CallServer(const Mail& callee, const std::string& method, const Message& message);
  void CallClient(const std::string& callee_str, const std::string& method, const Message& message);
  void CallServer(const std::string& callee_str, const std::string& method, const Message& message);
  void CallClientService(const std::string& method, const Message& message);
  void AddCallback(int timeout, Closure* closure);
};

}

#endif