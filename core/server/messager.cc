#include "messager.h"
#include "message_type.h"
#include "tcp_server.h"
#include "actor.h"
#include "worker.h"
#include "logger.h"
#include "actor_server.h"
#include "const.h"

using namespace boost::lockfree;
using namespace mtp;

void Messager::Init(const std::vector<std::string>& worker_ids) {
  for(auto& worker_id : worker_ids) {
    lockfree_queues_[worker_id] = new queue<BaseMessage*>(kRingBufferSize);
    queues_[worker_id] = new std::queue<BaseMessage*>;
  }
}

void Messager::WorkerInitActor(Actor& actor) {
  InitActorMessage* m = ActorServer::Instance().tcp_server().init_actor_pool().GetObject();
  m->Reset(&actor);
  m->set_allocator_id(kServerId);
  if (!lockfree_queues_[actor.worker_id()]->push(m)) {
    queues_[actor.worker_id()]->push(m);
  }
}

void Messager::WorkerClientJoin(const Mail& client_mail) {
  ClientJoinMessage* m = ActorServer::Instance().tcp_server().client_join_pool().GetObject();
  m->Reset(&client_mail);
  m->set_allocator_id(kServerId);
  if (!lockfree_queues_[client_mail.worker_id()]->push(m)) {
    queues_[client_mail.worker_id()]->push(m);
  }
}

void Messager::WorkerClientLeave(const Mail& client_mail) {
  ClientLeaveMessage* m = ActorServer::Instance().tcp_server().client_leave_pool().GetObject();
  m->Reset(&client_mail);
  m->set_allocator_id(kServerId);
  if (!lockfree_queues_[client_mail.worker_id()]->push(m)) {
    queues_[client_mail.worker_id()]->push(m);
  }
}

void Messager::WorkerLocalCall(const std::string& worker_id, LocalCallMessage& message) {
  if (!lockfree_queues_[worker_id]->push(&message)) {
    queues_[worker_id]->push(&message);
  }
}

void Messager::WorkerFreeObject(const std::string& worker_id, FreeObjectMessage& message) {
  if (!lockfree_queues_[worker_id]->push(&message)) {
    queues_[worker_id]->push(&message);
  }
}

void Messager::ServerFreeObject(FreeObjectMessage& message) {
  if (!lockfree_queues_[kServerId]->push(&message)) {
    queues_[kServerId]->push(&message);
  }
}

void Messager::ServerRemoteCall(RemoteCallMessage& message) {
  if (!lockfree_queues_[kServerId]->push(&message)) {
    queues_[kServerId]->push(&message);
  }
}

void Messager::ServerClientJoin(const Mail& client_mail) {
  ClientJoinMessage* m = ActorServer::Instance().tcp_server().client_join_pool().GetObject();
  m->Reset(&client_mail);
  m->set_allocator_id(kServerId);
  if (!lockfree_queues_[kServerId]->push(m)) {
    queues_[kServerId]->push(m);
  }
}

void Messager::TransferToLockFreeQueue() {
  for(auto& kv : queues_) {
    while(!kv.second->empty()) {
      BaseMessage* m = kv.second->front();
      if (lockfree_queues_[kv.first]->push(m)) {
        kv.second->pop();
      } else {
        break;
      }
    }
  }
}

void Messager::ConsumeWorkerMessage(Worker& worker) {
  BaseMessage* m = NULL;
  int message_proceed = 0;
  while (lockfree_queues_[worker.worker_id()]->pop(m)) {
    if (m != NULL) {
      m->Process(worker);
      worker.ReturnObjectToAllocator(m);
    }
    if (++message_proceed >= kMaxMessageProceedPerTick) {
      return;
    }
  }
}

void Messager::ConsumeServerMessage(TcpServer& server) {
  BaseMessage* m = NULL;
  int message_proceed = 0;
  while (lockfree_queues_[kServerId]->pop(m)) {
    if (m != NULL) {
      m->Process(server);
	  server.ReturnObjectToAllocator(m);
    }
    if (++message_proceed >= kMaxMessageProceedPerTick) {
      return;
    }
  }
}
