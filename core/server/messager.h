#ifndef MESSAGER_H__
#define MESSAGER_H__

#include <unordered_map>
#include <queue>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <vector>
#include <iostream>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "actor.pb.h"
#include "mail.h"
#include "const.h"
#include "message_type.h"

using namespace google::protobuf;
using namespace boost::lockfree;

namespace mtp {

class Worker;
class Actor;
class TcpServer;

class Messager {
 public:

  static Messager& Instance() {
    static Messager messager;
    return messager;
  }

  ~Messager() {
    for(auto& kv : lockfree_queues_) {
      delete kv.second;
    }
    for(auto& kv : queues_) {
      delete kv.second;
    }
  }

  void Init(const std::vector<std::string>& worker_ids);
  void WorkerInitActor(Actor& actor);
  void WorkerLocalCall(const std::string& worker_id, LocalCallMessage& message);
  void WorkerFreeObject(const std::string& worker_id, FreeObjectMessage& message);
  void WorkerClientJoin(const Mail& client_mail);
  void WorkerClientLeave(const Mail& client_mail);
  void ServerRemoteCall(RemoteCallMessage& message);
  void ServerFreeObject(FreeObjectMessage& message);
  void ServerClientJoin(const Mail& client_mail);
  void TransferToLockFreeQueue();
  void ConsumeWorkerMessage(Worker& worker);
  void ConsumeServerMessage(TcpServer& server);

 private:
  Messager() {
    lockfree_queues_[kServerId] = new queue<BaseMessage*>(kRingBufferSize);
    queues_[kServerId] = new std::queue<BaseMessage*>;
  }

  std::unordered_map<std::string, queue<BaseMessage*>*> lockfree_queues_;
  std::unordered_map<std::string, std::queue<BaseMessage*>*> queues_;
};

}

#endif
