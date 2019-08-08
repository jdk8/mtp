#ifndef ET_EPOLLER_H__
#define ET_EPOLLER_H__

#include "epoller.h"

namespace mtp {

class EtEpoller : public Epoller {
 public:
   void HandleWrite(int conn_fd, std::string& usrbuf);
   void InitEpollSocket(int conn_fd);
};

}

#endif