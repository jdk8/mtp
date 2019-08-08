#ifndef WORK_H__
#define WORK_H__

#include "epoller.h"
#include "actor.pb.h"
#include <queue>
#include <thread>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "timer.h"
#include "mail.h"
#include "message_type.h"
#include "logger.h"
#include "object_pool.h"

namespace mtp {

class Actor;

class Worker : public Timer {
 public:
  Worker(const std::string& worker_id) : worker_id_(worker_id), message_proceed_(0) {}
  ~Worker();
  void Run();
  const std::string& worker_id() const { return worker_id_; }
  void InitActor(Actor& actor);
  void CallLocalActor(const std::string& actor_id, const std::string& method, const std::string& message);
  void ClientJoin(const Mail& mail) const;
  void ClientLeave(const Mail& mail) const;
  void ReturnObjectToAllocator(BaseMessage* m);
  void FreeObject(const std::vector<BaseMessage*>& free_objects);
  ObjectPool<LocalCallMessage>& local_call_pool() { return local_call_pool_; }
  ObjectPool<RemoteCallMessage>& remote_call_pool() { return remote_call_pool_; }
  ObjectPool<FreeObjectMessage>& free_object_pool() { return free_object_pool_; }

 private:
  std::string worker_id_;
  std::unordered_map<std::string, Actor*> actors_;
  std::unordered_map<std::string, std::vector<BaseMessage*>*> free_objects_;
  ObjectPool<LocalCallMessage> local_call_pool_;
  ObjectPool<RemoteCallMessage> remote_call_pool_;
  ObjectPool<FreeObjectMessage> free_object_pool_;
  uint64_t message_proceed_;
};

}

#endif