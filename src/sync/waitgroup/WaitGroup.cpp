#include "WaitGroup.hpp"

namespace ayan::sync {

// ------------------------- WaitGroup Public Methods -------------------------

void WaitGroup::add(size_t count) {
  counter.fetch_add(count, std::memory_order_release);
}

void WaitGroup::done() {
  mutex.lock();
  counter.fetch_sub(1, std::memory_order_release);
  if (counter.load(std::memory_order_acquire) == 0){
    counter_is_zero.notify_all();
  }
  mutex.unlock();
}

void WaitGroup::wait() {
  mutex.lock();
  while (counter.load(std::memory_order_acquire) > 0){
    counter_is_zero.wait(mutex);
  }
  mutex.unlock();
}

} // namespace ayan::sync;
