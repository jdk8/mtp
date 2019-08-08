#include "actor.h"
#include "helper.h"
#include "worker.h"
#include "reflect.h"
#include <sstream>
#include <queue>
#include "messager.h"
#include "logger.h"
#include "client_actor.h"
#include "actor_client.h"

using namespace mtp;

void ClientActor::CallClient(const std::string& callee_str, const std::string& method, const Message& message) {
  Mail callee(callee_str);
  CallClient(callee, method, message);
}

void ClientActor::CallServer(const std::string& callee_str, const std::string& method, const Message& message) {
  Mail callee(callee_str);
  CallServer(callee, method, message);
}

void ClientActor::CallClient(const Mail& callee, const std::string& method, const Message& message) {
  ActorMessage req;
  req.set_callee_addr(callee.SerializeAsString());
  req.set_type(kActorMessageTypeCallClient);
  req.set_method(method);
  req.set_message(message.SerializeAsString());
  ActorClient::Instance().server_stub()->SendActorMessage(NULL, &req, NULL, NULL);
}

void ClientActor::CallServer(const Mail& callee, const std::string& method, const Message& message) {
  ActorMessage req;
  req.set_callee_addr(callee.SerializeAsString());
  req.set_type(kActorMessageTypeCallServer);
  req.set_method(method);
  req.set_message(message.SerializeAsString());
  ActorClient::Instance().server_stub()->SendActorMessage(NULL, &req, NULL, NULL);
}

void ClientActor::AddCallback(int timeout, Closure* closure) {
  timer()->AddCallback(timeout, closure);
}

void ClientActor::CallClientService(const std::string& method, const Message& message) {
  CallServer(*ActorClient::Instance().client_service_mail(), method, message);
}