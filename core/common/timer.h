#ifndef TIMER_H__
#define TIMER_H__

#include <google/protobuf/stubs/callback.h>
#include <queue>
#include <sys/timeb.h>
#include <iostream>
#include "helper.h"

using namespace google::protobuf;

namespace mtp {

struct TimerCallback {
 public:
  TimerCallback(
    uint64_t start_time,
    int timeout,
    Closure* closure)
  : start_time_(start_time),
    timeout_(timeout),
    closure_(closure) {}

  uint64_t do_time() { return start_time_ + timeout_; }
  Closure* closure() { return closure_; }

 private:
  uint64_t start_time_;
  int timeout_;
  uint64_t do_time_;
  Closure* closure_;
};

class Timer {
 public:
  void AddCallback(int timeout, Closure* closure);

 protected:
  void TickTimer();

 private:
  std::deque<TimerCallback*> callbacks_;
};

}

#endif