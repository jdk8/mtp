#ifndef USER_SERVICE_H__
#define USER_SERVICE_H__

#include <mtp/server_actor.h>
#include <unordered_map>
#include <mtp/mail.h>

class UserService : public mtp::ClientService {
 public:
  UserService() {
    set_instance(4);
  }

  void Init() {
    LOG_INFO << "[UserService]UserService Init";
  }

  void RequestRoom(const RoomInfo& room_info) {
    if (rooms_.size() == 0) {
      Actor* room = NewLocalActor("Room");
      rooms_[room_info.creator()] = room->mail();
      const_cast<RoomInfo&>(room_info).set_room_mail(room->mail()->SerializeAsString());
      CallClient(room_info.creator_mail(), "RequestRoomResult", room_info);
      for (int i = 0; i < instance(); i++) {
        if (i != shard_no()) {
          CallShardService("UserService", i, "AddRoom", room_info);
        }
      }
    } else {
      int index = rand() % rooms_.size();
      auto iter = rooms_.begin();
      std::advance(iter, index);
      const_cast<RoomInfo&>(room_info).set_creator(iter->first);
      const_cast<RoomInfo&>(room_info).set_room_mail(iter->second->SerializeAsString());
      CallClient(room_info.creator_mail(), "RequestRoomResult", room_info);
    }
  }

  void AddRoom(const RoomInfo& room_info) {
    LOG_INFO << "[UserService]AddRoom, creator is:" << room_info.creator();
    rooms_[room_info.creator()] = new mtp::Mail(room_info.room_mail());
  }

  void OnClientJoin(const mtp::Mail& mail) {
    LOG_INFO << "[UserService]OnClientJoin";
  }

  void OnClientLeave(const mtp::Mail& mail) {
    LOG_INFO << "[UserService]OnClientLeave";
  }

 private:
  std::unordered_map<std::string, mtp::Mail*> rooms_;
};

#endif
