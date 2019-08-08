#ifndef OBJECT_POOL_H__
#define OBJECT_POOL_H__

#include <unordered_set>
#include "logger.h"
#include "const.h"

namespace mtp {

template<typename T>
class ObjectPool {
 public:
  T* GetObject() {
    T* obj = NULL;
    if (free_objs_.size() == 0) {
      obj = new T;
    } else {
      obj = *(free_objs_.begin());
      free_objs_.erase(obj);
    }
    occupied_objs_.insert(obj);
    return obj;
  }

  void ReleaseObject(T* obj) {
    occupied_objs_.erase(obj);
    if (free_objs_.size() > kMaxFreeObjectCollect) {
      delete obj;
    } else {
      free_objs_.insert(obj);
    }
  }

  void Debug(const std::string& owner_id) {
    LOG_DEBUG << "[GC]" << this << "," << owner_id << ",occupied_objs:" << occupied_objs_.size()
      << ", free_objs:" << free_objs_.size();
  }

 private:
  std::unordered_set<T*> occupied_objs_;
  std::unordered_set<T*> free_objs_;
};

}

#endif