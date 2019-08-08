#include "epoller.h"
#include "messager.h"
#include "const.h"

using namespace mtp;

Epoller::Epoller() {
  event_ = (epoll_event*)calloc (1, sizeof event_);
  events_ = (epoll_event*)calloc (kMaxEpollEvent, sizeof event_);
  CreateEpoll();
}

Epoller::~Epoller() {
  free(event_);
  free(events_);
}

int Epoller::CreateEpoll() {
  if ((epoll_fd_ = epoll_create1(0)) == -1) {
   LOG_ERROR << "Error in epoll_create\n";
   return -1;
  }
  return 1;
}

int Epoller::SetSocketNonBlock(int conn_fd) {
  int flags;

  if ((flags = fcntl (conn_fd, F_GETFL, 0)) == -1) {
    LOG_ERROR << "Error in SetSocketNonBlock1\n";
    return -1;
  }

  flags |= O_NONBLOCK;
  if (fcntl (conn_fd, F_SETFL, flags) == -1) {
    LOG_ERROR << "Error in SetSocketNonBlock2\n";
    return -1;
  }
  return 1;
}

int Epoller::AddEpollEvent(int fd, uint32_t events) {
  event_->data.fd = fd;
  event_->events = events;
  if (epoll_ctl (epoll_fd_, EPOLL_CTL_ADD, fd, event_) == -1) {
    LOG_ERROR << "Error in AddEpollEvent\n";
    return -1;
  }
  return 1;
}

int Epoller::DelEpollEvent(int fd, uint32_t events) {
  event_->data.fd = fd;
  event_->events = events;
  if (epoll_ctl (epoll_fd_, EPOLL_CTL_DEL, fd, event_) == -1) {
    LOG_ERROR << "Error in DelEpollEvent\n";
    return -1;
  }
  return 1;
}

int Epoller::EpollWaitEvent() {
  return epoll_wait (epoll_fd_, events_, kMaxEpollEvent, kEpollWaitTimeout);
}

void Epoller::HandleRead(int conn_fd, std::string& usrbuf) {
  char read_buf[1024];
  ssize_t nread;
  while (1) {
    nread = read(conn_fd, read_buf, sizeof read_buf);
    if (nread < 0) {
      if (errno == EINTR) {
        nread = 0;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK){
        break;
      } else {
        LOG_INFO << "Other read error, errno=" << errno;
        Close(conn_fd);
        break;
      }
    } else if (nread == 0) {
      Close(conn_fd);
      break;
    }
    usrbuf.append(read_buf, nread);
  }
}

void Epoller::Close(int conn_fd) {
  LOG_INFO << "Closed connection: " << conn_fd;
  close(conn_fd);
  HandleClose(conn_fd);
}

void Epoller::TickMessage() {
}

void Epoller::Run() {
  while (1) {
    int n = EpollWaitEvent();
    TickTimer();
    TickMessage();
    for (int i = 0; i < n; i++) {
      int event_fd = events_[i].data.fd;
      int event = events_[i].events;
      HandleEvent(event_fd, event);
    }
  }
}
