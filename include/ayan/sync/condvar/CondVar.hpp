#pragma once

#include <atomic>

namespace ayan::sync {

// forward-declaration:
class Mutex;

class ConditionVar {
private: // fields:
  std::atomic<uint32_t> generation_counter = 0;
  std::atomic<uint32_t> waiters_count = 0;

public: // methods:
  ConditionVar() = default;
  ~ConditionVar() = default;

  void wait(Mutex& mutex) noexcept;
  void notify_one() noexcept;
  void notify_all() noexcept;
}; // class ConditionVar;

} // namespace ayan::sync;
