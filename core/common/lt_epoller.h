#ifndef LT_EPOLLER_H__
#define LT_EPOLLER_H__

#include "epoller.h"
#include <unordered_map>

namespace mtp {

class LtEpoller : public Epoller {
 public:
  void HandleWrite(int conn_fd, std::string& usrbuf);
  void InitEpollSocket(int conn_fd);

 private:
  std::unordered_map<int, bool> is_epollouts_;
};

}

#endif