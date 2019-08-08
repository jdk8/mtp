#include "timer.h"

using namespace mtp;

void Timer::TickTimer() {
  while(!callbacks_.empty()) {
    TimerCallback* c = callbacks_.front();
    if (c->do_time() > TimeHelper::CurrentTimeMillis()) {
      break;
    }
    c->closure()->Run();
    callbacks_.pop_front();
  }
}

void Timer::AddCallback(int timeout, Closure* closure) {
  uint64_t curr_time = TimeHelper::CurrentTimeMillis();
  callbacks_.push_back(new TimerCallback(curr_time, timeout, closure));
}