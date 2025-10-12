#pragma once

#include <ayan/sync/mutex/Mutex.hpp>

namespace ayan::sync {

// RAII-wrapper for Mutex:
class UniqueLock {
private: // fields:
  Mutex* mutex_ptr = nullptr;
  bool owns_lock = false;

public: // methods:
  UniqueLock() noexcept = default;
  explicit UniqueLock(Mutex&) noexcept;
  ~UniqueLock() noexcept;

  UniqueLock(const UniqueLock&) = delete;
  UniqueLock& operator=(const UniqueLock&) = delete;

  UniqueLock(UniqueLock&&) noexcept;
  UniqueLock& operator=(UniqueLock&&) noexcept;

  void lock();
  void unlock();
}; // class UniqueLock;

} // namespace ayan::sync;
