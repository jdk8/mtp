#include "lt_epoller.h"

using namespace mtp;

void LtEpoller::HandleWrite(int conn_fd, std::string& usrbuf) {
  ssize_t n = usrbuf.length();
  ssize_t nwriten, nleft = n;
  while(true) {
    nwriten = write(conn_fd, usrbuf.substr(n - nleft).c_str(), nleft);
    if (nwriten < 0) {
      if (errno == EINTR) {
        nwriten = 0;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_INFO << "LT write buffer is full, register EPLLOUT";
        is_epollouts_[conn_fd] = true;
        AddEpollEvent(conn_fd, EPOLLOUT);
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
  if (nleft == 0) {
    auto iter = is_epollouts_.find(conn_fd);
    if (iter != is_epollouts_.end() && iter->second) {
      LOG_INFO << "Delete EPOLLOUT";
      DelEpollEvent(conn_fd, EPOLLOUT);
      iter->second = false;
    }
  }
  usrbuf = usrbuf.substr(n - nleft);
}

void LtEpoller::InitEpollSocket(int conn_fd) {
  AddEpollEvent(conn_fd, EPOLLIN);
}
