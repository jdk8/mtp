// Generated code

#include <mtp/reflect.h>
#include "user.h"
#include "room.pb.h"

namespace mtp {

Actor* NewActor(const std::string& actor_name) {
  if (actor_name == "User")
    return new User;
  return NULL;
}

void CallActorMethod(Actor* actor, const std::string& method, const std::string& message) {
  if (method == "SomeoneJoinRoom") {
    JoinInfo arg;
    arg.ParseFromString(message);
    static_cast<User*>(actor)->SomeoneJoinRoom(arg);
    return;
  }
  if (method == "RequestRoomResult") {
    RoomInfo arg;
    arg.ParseFromString(message);
    static_cast<User*>(actor)->RequestRoomResult(arg);
    return;
  }
  if (method == "RecvChat") {
    ChatInfo arg;
    arg.ParseFromString(message);
    static_cast<User*>(actor)->RecvChat(arg);
    return;
  }
}

}