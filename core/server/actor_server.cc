#include "actor_server.h"
#include "const.h"

using namespace mtp;

void ActorServiceImpl::SendActorMessage(google::protobuf::RpcController* controller,
                        const ActorMessage* request,
                        ActorMessage* response,
                        google::protobuf::Closure* done) {
  Mail callee;
  callee.ParseFromString(request->callee_addr());
  if (request->type() == kActorMessageTypeCallServer){
    LocalCallMessage* m = ActorServer::Instance().tcp_server().local_call_pool().GetObject();
    m->Reset(callee.actor_id(), request->method(), request->message());
    m->set_allocator_id(kServerId);
    Messager::Instance().WorkerLocalCall(callee.worker_id(), *m);
  } else if (request->type() == kActorMessageTypeCallClient) {
    ActorClientService* stub = ActorServer::Instance().GetClientStub(callee.conn_fd());
    if (stub) {
      stub->SendActorMessage(NULL, request, NULL, NULL);
    }
  } else if (request->type() == kActorMessageTypeClientJoin) {
    Mail* client_mail = new Mail(request->callee_addr());
    Messager::Instance().WorkerClientJoin(*client_mail);
    Messager::Instance().ServerClientJoin(*client_mail);
  }
}

ActorServer::~ActorServer() {
  for(auto& kv : service_mails_) {
    delete kv.second;
  }
}

void ActorServer::Init(const std::string& ip, int port, const std::string& actor_dir, int worker_num) {
  worker_num_ = worker_num;
  Init(ip, port, actor_dir);
}

void ActorServer::Init(const std::string& ip, int port, const std::string& actor_dir) {
  ip_ = ip;
  port_ = port;
  actor_dir_ = actor_dir;
  tcp_server_.ExportOnPort(std::to_string(port).c_str(), &service_impl_);
  for (int i = 0; i < kWorkerNum; i++) {
    worker_ids_.push_back("Worker#" + std::to_string(i));
  }
  Messager::Instance().Init(worker_ids_);
  InitServiceActors();
  StartWorkerThreads();
}

void ActorServer::InitServiceActors() {
  LOG_INFO << "InitServiceActors";
  std::vector<std::string> files = FileHelper::GetFileList(actor_dir_);
  for (auto& f: files) {
    LOG_INFO << "Scanning: " << f;
    std::string actor_name;
    for (auto& s : StringHelper::Split(StringHelper::Split(f, ".")[0], "_")) {
      s[0] -= 32;
      actor_name += s;
    }
    Actor* tmp_actor = NewActor(actor_name);
    if (ServiceActor* service_actor = dynamic_cast<ServiceActor*>(tmp_actor)) {
      bool is_client_service = false;
      if (dynamic_cast<ClientService*>(tmp_actor)) {
        is_client_service = true;
      }
      if (is_client_service) {
        client_service_.set_service_name(actor_name);
      }
      std::vector<Mail*>* service_mails = new std::vector<Mail*>;
      for (int i = 0; i < service_actor->instance(); i++) {
        std::string actor_id = actor_name + "#" + std::to_string(i);
        Actor* actor = NewActor(actor_name);
        const std::string& worker_id = worker_ids_[rand() % worker_ids_.size()];
        Mail* service_mail = new Mail(ip_, port_, actor_id, worker_id);
        actor->set_mail(service_mail);
        dynamic_cast<ServiceActor*>(actor)->set_shard_no(i);
        service_mails->push_back(service_mail);
        Messager::Instance().WorkerInitActor(*actor);
        if (is_client_service) {
          client_service_.add_service_mails(service_mail->SerializeAsString());
        }
      }
      service_mails_[actor_name] = service_mails;
    }
    LOG_INFO << "Actor name:" << actor_name << ", mail is:" << tmp_actor->mail();
    delete tmp_actor;
  }
  LOG_INFO << "InitServiceActors done";
}

void ActorServer::StartWorkerThreads() {
  for (auto& worker_id : worker_ids_) {
    worker_threads_.push_back(new std::thread(&Worker::Run, new Worker(worker_id)));
  }
}

void ActorServer::RunForever() {
  tcp_server_.Run();
  for (auto& t : worker_threads_) {
    t->join();
  }
}

Mail* ActorServer::RandServiceMail(const std::string& service_name) {
  auto iter = service_mails_.find(service_name);
  if (iter != service_mails_.end()) {
    std::vector<Mail*>* mails = iter->second;
    return (*mails)[rand() % mails->size()];
  }
  return NULL;
}

Mail* ActorServer::ShardServiceMail(const std::string& service_name, uint32_t shard_no) {
  auto iter = service_mails_.find(service_name);
  if (iter != service_mails_.end()) {
    std::vector<Mail*>* mails = iter->second;
    if (shard_no < mails->size()) {
      return (*mails)[shard_no];
    }
  }
  return NULL;
}

Mail* ActorServer::ShardServiceMail(const std::string& service_name, const std::string& shard_key) {
  auto iter = service_mails_.find(service_name);
  if (iter != service_mails_.end()) {
    std::vector<Mail*>* mails = iter->second;
    return (*mails)[CrcHelper::Crc16(shard_key) % mails->size()];
  }
  return NULL;
}

ActorClientService* ActorServer::GetClientStub(int conn_fd) {
  auto iter = client_stubs_.find(conn_fd);
  if (iter != client_stubs_.end()) {
    return iter->second;
  }
  return NULL;
}

void ActorServer::SetClientStub(int conn_fd, ActorClientService* stub) {
  client_stubs_[conn_fd] = stub;
}

void ActorServer::DelClientStub(int conn_fd) {
  auto iter = client_stubs_.find(conn_fd);
  if (iter != client_stubs_.end()) {
    delete iter->second;
    client_stubs_.erase(iter);
  }
}