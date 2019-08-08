#ifndef MAIL_H__
#define MAIL_H__

#include <iostream>
#include "actor.pb.h"
#include "logger.h"
#include "const.h"

namespace mtp {

class Mail {
 public:
  Mail() {};
  Mail(std::string serialized_string);
  Mail(std::string ip, int port, std::string actor_id, std::string worker_id);
  Mail(std::string ip, int port, std::string actor_id, int conn_fd, std::string worker_id);

  std::string SerializeAsString() const;
  void ParseFromString(std::string s);

  const std::string& ip() const { return serializable_mail_.ip(); }
  int port() const { return serializable_mail_.port(); }
  bool is_server() const { return serializable_mail_.is_server(); }
  const std::string& actor_id() const { return serializable_mail_.actor_id(); }
  int conn_fd() const { return serializable_mail_.conn_fd(); }
  const std::string& worker_id() const { return serializable_mail_.worker_id(); }
  void set_actor_id(std::string actor_id) { serializable_mail_.set_actor_id(actor_id); }

 private:
  SerializableMail serializable_mail_;
};

}

#endif
