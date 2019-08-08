#include "helper.h"
#include "worker.h"
#include "actor.h"
#include "reflect.h"
#include "messager.h"
#include "logger.h"
#include "reflect.h"
#include "const.h"

using namespace mtp;

Worker::~Worker() {
  for(auto& kv : free_objects_) {
    delete kv.second;
  }
}

void Worker::Run() {
  while (true) {
    Messager::Instance().ConsumeWorkerMessage(*this);
    TickTimer();
    std::this_thread::sleep_for(std::chrono::milliseconds(KWorkerTickInterval));
  }
}

void Worker::InitActor(Actor& actor) {
  if (StringHelper::Split(actor.actor_id(), "#").size() > 1) {
    LOG_INFO << "Worker[" << worker_id_ << "] init actor " << actor.actor_id();
  }
  actors_[actor.actor_id()] = &actor;
  actor.set_worker(this);
  actor.Init();
}

void Worker::CallLocalActor(const std::string& actor_id, const std::string& method, const std::string& message) {
  auto iter = actors_.find(actor_id);
  if(iter != actors_.end()) {
     CallActorMethod(iter->second, method, message);
  } else {
    LOG_ERROR << "[Worker::CallLocalActor] " << worker_id_ << "|" << actor_id << "|" << method << "|" << message;
  }
}

void Worker::ClientJoin(const Mail& mail) const {
  auto iter = actors_.find(mail.actor_id());
  if(iter != actors_.end()) {
    CallClientService(iter->second, "ClientJoin", mail);
  }
}

void Worker::ClientLeave(const Mail& mail) const {
  auto iter = actors_.find(mail.actor_id());
  if(iter != actors_.end()) {
    CallClientService(iter->second, "ClientLeave", mail);
  }
}

void Worker::ReturnObjectToAllocator(BaseMessage* m) {
  const std::string& allocator_id = m->allocator_id();
  auto iter = free_objects_.find(allocator_id);
  if (iter == free_objects_.end()) {
    free_objects_[allocator_id] = new std::vector<BaseMessage*>;
  }
  free_objects_[allocator_id]->push_back(m);
  if (free_objects_[allocator_id]->size() > kMaxFreeObjectReturn && allocator_id != "") {
    FreeObjectMessage* m = free_object_pool_.GetObject();
    m->Reset(*free_objects_[allocator_id]);
    m->set_allocator_id(worker_id_);
    if (allocator_id == kServerId) {
      Messager::Instance().ServerFreeObject(*m);
    } else {
      Messager::Instance().WorkerFreeObject(allocator_id, *m);
    }
    free_objects_[allocator_id]->clear();
  }
}

void Worker::FreeObject(const std::vector<BaseMessage*>& free_objects) {
  for (auto& m : free_objects) {
    m->Release(*this);
  }
}