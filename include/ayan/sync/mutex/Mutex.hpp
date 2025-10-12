#pragma once

#include <atomic>

namespace ayan::sync {

class Mutex {
private: // types:
  enum class MutexState {
    Unlocked, // free, ready for be capture;
    Locked, // captured by single thread;
    Contended // locked, there are waiting threads;
  };

private: // fields:
  std::atomic<MutexState> state = MutexState::Unlocked;

public: // methods:
  Mutex() = default;
  ~Mutex() = default;

  Mutex(const Mutex&) = delete;
  Mutex& operator=(const Mutex&) = delete;
  Mutex(Mutex&&) = delete;
  Mutex& operator=(Mutex&&) = delete;

  void lock() noexcept;
  void unlock() noexcept;
}; // class Mutex;

} // namespace ayan::sync;

// TODO: Mutex.try_lock(), Mutex.try_lock_for(timeout);

#include <ayan/sync/detail/mutex/LockGuard.hpp>
#include <ayan/sync/detail/mutex/UniqueLock.hpp>
