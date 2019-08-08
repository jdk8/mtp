#ifndef USER_H__
#define USER_H__

#include <mtp/client_actor.h>
#include "room.pb.h"
#include <mtp/helper.h>
#include <google/protobuf/stubs/callback.h>

using namespace google::protobuf;

class User : public mtp::ClientActor {
 public:
  void Init() {
    LOG_INFO << "[User]User Init";
    AddCallback(1000, NewCallback(this, &User::Begin));
  }

  void Begin() {
    nickname_ = "User#" + std::to_string(mtp::TimeHelper::CurrentTimeMillis());
    RoomInfo room_info;
    room_info.set_creator(nickname_);
    room_info.set_creator_mail(mail()->SerializeAsString());
    // Request a room, create one if no room
    CallClientService("RequestRoom", room_info);
  }

  void RequestRoomResult(const RoomInfo& room_info) {
    LOG_INFO << "[User]RequestRoomResult, room creator is:" << room_info.creator();
    JoinInfo join_info;
    join_info.set_nickname(nickname_);
    join_info.set_mail(mail()->SerializeAsString());
    CallServer(room_info.room_mail(), "JoinRoom", join_info);
  }

  void SomeoneJoinRoom(const JoinInfo& join_info) {
    LOG_INFO << "[User]SomeoneJoinRoom, joiner is:" << join_info.nickname();
    ChatInfo chat_info;
    chat_info.set_sender(nickname_);
    chat_info.set_content("I'm " + nickname_ + ", welcome in this room");
    CallClient(join_info.mail(), "RecvChat", chat_info);
  }

  void RecvChat(const ChatInfo& chat_info) {
    LOG_INFO << "[User]RecvChat, content is: " << chat_info.content();
  }

 private:
  std::string nickname_;
};

#endif