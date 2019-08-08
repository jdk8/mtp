// Generated code

#include <mtp/reflect.h>
#include "room.h"
#include "user_service.h"
#include "room.pb.h"

namespace mtp {

Actor* NewActor(const std::string& actor_name) {
  if (actor_name == "Room")
    return new Room;
  if (actor_name == "UserService")
    return new UserService;
  return NULL;
}

void CallActorMethod(Actor* actor, const std::string& method, const std::string& message) {
  if (method == "JoinRoom") {
    JoinInfo arg;
    arg.ParseFromString(message);
    static_cast<Room*>(actor)->JoinRoom(arg);
    return;
  }
  if (method == "AddRoom") {
    RoomInfo arg;
    arg.ParseFromString(message);
    static_cast<UserService*>(actor)->AddRoom(arg);
    return;
  }
  if (method == "OnClientLeave") {
    mtp::Mail arg;
    arg.ParseFromString(message);
    static_cast<UserService*>(actor)->OnClientLeave(arg);
    return;
  }
  if (method == "OnClientJoin") {
    mtp::Mail arg;
    arg.ParseFromString(message);
    static_cast<UserService*>(actor)->OnClientJoin(arg);
    return;
  }
  if (method == "RequestRoom") {
    RoomInfo arg;
    arg.ParseFromString(message);
    static_cast<UserService*>(actor)->RequestRoom(arg);
    return;
  }
}

void CallClientService(Actor* actor, const std::string& method, const Mail& mail) {
  if (method == "ClientJoin") {
    static_cast<ClientService*>(actor)->ClientJoin(mail);
    return;
  }
  if (method == "ClientLeave") {
    static_cast<ClientService*>(actor)->ClientLeave(mail);
    return;
  }
}

}