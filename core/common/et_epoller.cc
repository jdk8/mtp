#include "et_epoller.h"

using namespace mtp;

void EtEpoller::HandleWrite(int conn_fd, std::string& usrbuf) {
  ssize_t n = usrbuf.length();
  ssize_t nwriten, nleft = n;
  while (true) {
    nwriten = write(conn_fd, usrbuf.substr(n - nleft).c_str(), nleft);
    if (nwriten < 0) {
      if (errno == EINTR) {
        nwriten = 0;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_INFO << "Write buffer is full, waiting for EPOLLOUT";
        break;
      } else {
        LOG_INFO << "Other write error, errno=" << errno;
        Close(conn_fd);
        break;
      }
    }
    nleft -= nwriten;
    if (nleft == 0) {
      break;
    }
  }
  usrbuf = usrbuf.substr(n - nleft);
}

void EtEpoller::InitEpollSocket(int conn_fd) {
  AddEpollEvent(conn_fd, EPOLLIN | EPOLLOUT | EPOLLET);
}
