#ifndef TCP_SERVER_H__
#define TCP_SERVER_H__

#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <memory>
#include <utility>
#include <google/protobuf/service.h>
#include "myrpc_channel.h"
#include "epoller.h"
#include "lt_epoller.h"
#include "et_epoller.h"
#include "mail.h"
#include "message_type.h"
#include "object_pool.h"
#include "const.h"

using namespace google::protobuf;

namespace mtp {

class ActorServer;

class TcpServer: public LtEpoller {
 public:
  ~TcpServer();
  void ExportOnPort (const char* port, Service* service);
  std::string& GetWriteBuffer(int conn_fd);
  void HandleEvent(int event_fd, int event);
  void TickMessage();
  void HandleClose(int conn_fd);
  void ClientJoin(int conn_fd, const Mail& mail);
  void SendMessage(int conn_fd, const ActorMessage& message) const;
  ObjectPool<LocalCallMessage>& local_call_pool() { return local_call_pool_; }
  ObjectPool<InitActorMessage>& init_actor_pool() { return init_actor_pool_; }
  ObjectPool<ClientJoinMessage>& client_join_pool() { return client_join_pool_; }
  ObjectPool<ClientLeaveMessage>& client_leave_pool() { return client_leave_pool_; }
  ObjectPool<FreeObjectMessage>& free_object_pool() { return free_object_pool_; }
  void ReturnObjectToAllocator(BaseMessage* m);
  void FreeObject(const std::vector<BaseMessage*>& free_objects);
  const std::string& server_id() { return kServerId; }

 private:
  void HandleAccept(int conn_fd);

  int listen_fd_;
  const char* port_;
  std::unordered_map<int, MyRpcChannel*> rpc_channels_;
  Service* service_;
  std::unordered_map<int, std::string> write_bufs_;
  std::unordered_map<int, const Mail*> client_mails_;
  ObjectPool<LocalCallMessage> local_call_pool_;
  ObjectPool<InitActorMessage> init_actor_pool_;
  ObjectPool<ClientJoinMessage> client_join_pool_;
  ObjectPool<ClientLeaveMessage> client_leave_pool_;
  ObjectPool<FreeObjectMessage> free_object_pool_;
  std::unordered_map<std::string, std::vector<BaseMessage*>*> free_objects_;
};

}

#endif
