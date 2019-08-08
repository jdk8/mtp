#ifndef ROOM_H__
#define ROOM_H__

#include <mtp/server_actor.h>
#include "room.pb.h"
#include <unordered_map>

class Room : public mtp::ServerActor {
 public:
   void Init() {
     LOG_INFO << "[Room]Room Init";
   }

   void JoinRoom(const JoinInfo& join_info) {
     LOG_INFO << "[Room]JoinRoom, joiner: " << join_info.nickname();
     for (auto& kv : room_members_) {
       CallClient(*kv.second, "SomeoneJoinRoom", join_info);
     }
     room_members_[join_info.nickname()] = new mtp::Mail(join_info.mail());
   }
 private:
  std::unordered_map<std::string, mtp::Mail*> room_members_;
};

#endif
