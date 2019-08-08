#ifndef EPOLLER_H__
#define EPOLLER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
#include "timer.h"

namespace mtp {

class Epoller : public Timer {
 public:
  Epoller ();
  ~Epoller ();
  virtual void HandleWrite(int conn_fd, std::string& usrbuf) = 0;
  virtual void InitEpollSocket(int conn_fd) = 0;
  virtual std::string& GetWriteBuffer(int conn_fd) = 0;
  virtual void HandleEvent(int event_fd, int event) = 0;
  virtual void TickMessage();
  virtual void HandleClose(int conn_fd) = 0;
  void Run();

 protected:
  int CreateEpoll();
  int AddEpollEvent(int fd, uint32_t events);
  int DelEpollEvent(int fd, uint32_t events);
  int EpollWaitEvent();
  int SetSocketNonBlock(int conn_fd);
  void HandleRead(int conn_fd, std::string& usrbuf);
  void Close(int conn_fd);
  int epoll_fd_;
  struct epoll_event* event_;
  struct epoll_event *events_;
};

}

#endif
