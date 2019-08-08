#ifndef ACTOR_SERVER_H__
#define ACTOR_SERVER_H__

#include "helper.h"
#include "actor.pb.h"
#include "server_actor.h"
#include "reflect.h"
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <sstream>
#include "worker.h"
#include "tcp_server.h"
#include <boost/lockfree/spsc_queue.hpp>
#include "messager.h"
#include "logger.h"
#include <unordered_map>

namespace mtp {

class ActorServiceImpl : public ActorService {
 public:
  void SendActorMessage(google::protobuf::RpcController* controller,
                        const ActorMessage* request,
                        ActorMessage* response,
                        google::protobuf::Closure* done);
};

class ActorServer {
 public:
  static ActorServer& Instance() {
    static ActorServer actor_server;
    return actor_server;
  }
  ~ActorServer();

  void Init(const std::string& ip, int port, const std::string& actor_dir);
  void Init(const std::string& ip, int port, const std::string& actor_dir, int worker_num);
  void RunForever();
  const std::string& ip() { return ip_; }
  int port() { return port_; }
  const ServiceAddr& client_service() { return client_service_; }
  Mail* RandServiceMail(const std::string& service_name);
  Mail* ShardServiceMail(const std::string& service_name, uint32_t shard_no);
  Mail* ShardServiceMail(const std::string& service_name, const std::string& shard_key);
  ActorClientService* GetClientStub(int conn_fd);
  void SetClientStub(int conn_fd, ActorClientService* stub);
  void DelClientStub(int conn_fd);
  TcpServer& tcp_server() { return tcp_server_; }

 private:
  ActorServer() : worker_num_(kWorkerNum) {}
  void InitServiceActors();
  void StartWorkerThreads();

  std::string ip_;
  int port_;
  std::string actor_dir_;
  std::vector<std::string> worker_ids_;
  std::vector<std::thread*> worker_threads_;
  std::unordered_map<int, ActorClientService*> client_stubs_;
  InitClient init_client_;
  std::unordered_map<std::string, std::vector<Mail*>*> service_mails_;
  ActorServiceImpl service_impl_;
  TcpServer tcp_server_;
  ServiceAddr client_service_;
  int worker_num_;
};

}

#endif