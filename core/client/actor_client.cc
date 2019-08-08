#include "actor_client.h"

using namespace mtp;

void ActorClient::Init(const std::string& ip, int port, const std::string& actor_dir) {
  ip_ = ip;
  port_ = port;
  actor_dir_ = actor_dir;
  tcp_client_.Connect(ip_, port_, &actor_client_service_impl);
  server_stub_ = new ActorService::Stub(tcp_client_.get_rpc_channel());
}

void ActorClient::RunForever() {
  tcp_client_.Run();
}

void ActorClient::InitClientActor(int conn_fd) {
  std::vector<std::string> files = FileHelper::GetFileList(actor_dir_);
  for (auto& f: files) {
    LOG_INFO << "Scanning: " << f;
    std::stringstream ss;
    for (auto& s : StringHelper::Split(StringHelper::Split(f, ".")[0], "_")) {
      s[0] -= 32;
      ss << s;
    }
    std::string actor_name = ss.str();
    client_actor_ = NewActor(actor_name);
    Mail* client_mail = new Mail(ip_, port_, client_service_mail_->actor_id(), conn_fd, client_service_mail_->worker_id());
    client_actor_->set_mail(client_mail);
    client_actor_->set_timer(&tcp_client_);
    client_actor_->Init();
    ActorMessage req;
    req.set_callee_addr(client_mail->SerializeAsString());
    req.set_type(kActorMessageTypeClientJoin);
    server_stub_->SendActorMessage(NULL, &req, NULL, NULL);
    break;
  }
}

void ActorClientServiceImpl::SendActorMessage(google::protobuf::RpcController* controller, const ActorMessage* request,
  ActorMessage* response, google::protobuf::Closure* done) {
  if (request->type() == kActorMessageTypeInitClient) {
    init_client_.ParseFromString(request->message());
    ServiceAddr client_service;
    client_service.ParseFromString(init_client_.client_service());
    std::string actor_name = client_service.service_name();
    Mail* service_mail = new Mail(client_service.service_mails(rand() % client_service.service_mails_size()));
    ActorClient::Instance().set_client_service_mail(service_mail);
    ActorClient::Instance().InitClientActor(init_client_.conn_fd());
  } else if (request->type() == kActorMessageTypeCallClient) {
    CallActorMethod(ActorClient::Instance().client_actor(), request->method(), request->message());
  }
}
