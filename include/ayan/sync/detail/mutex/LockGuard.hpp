#pragma once

#include <ayan/sync/mutex/Mutex.hpp>

namespace ayan::sync {

// RAII-wrapper for Mutex:
class LockGuard {
private: // fields:
  Mutex& mutex;

public: // methods:
  explicit LockGuard(Mutex& m) noexcept;
  ~LockGuard() noexcept;

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;

  LockGuard(LockGuard&&) = delete;
  LockGuard& operator=(LockGuard&&) = delete;
};

} // namespace ayan::sync;
