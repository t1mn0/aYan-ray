#pragma once

#include <ayan/sync/mutex/Mutex.hpp>
#include <ayan/sync/condvar/CondVar.hpp>

#include <atomic>

namespace ayan::sync {

class WaitGroup {
private: // fields:
  std::atomic<size_t> counter;
  Mutex mutex;
  ConditionVar counter_is_zero;

public: // methods:
  void add(size_t count);
  void done();
  void wait();
}; // class WaitGroup;

} // namespace ayan::sync;
