#include "helper.h"
#include <iostream>
#include "mail.h"
#include <sstream>
#include "logger.h"

using namespace mtp;

Mail::Mail(std::string serialized_string) {
  ParseFromString(serialized_string);
}

Mail::Mail(std::string ip,
  int port,
  std::string actor_id,
  std::string worker_id) {
  serializable_mail_.set_is_server(true);
  serializable_mail_.set_ip(ip);
  serializable_mail_.set_port(port);
  serializable_mail_.set_actor_id(actor_id);
  serializable_mail_.set_worker_id(worker_id);
};

Mail::Mail(std::string ip,
  int port,
  std::string actor_id,
  int conn_fd,
  std::string worker_id) {
  serializable_mail_.set_is_server(false);
  serializable_mail_.set_ip(ip);
  serializable_mail_.set_port(port);
  serializable_mail_.set_actor_id(actor_id);
  serializable_mail_.set_conn_fd(conn_fd);
  serializable_mail_.set_worker_id(worker_id);
};

std::string Mail::SerializeAsString() const {
  return serializable_mail_.SerializeAsString();
}

void Mail::ParseFromString(std::string s) {
  serializable_mail_.ParseFromString(s);
}

