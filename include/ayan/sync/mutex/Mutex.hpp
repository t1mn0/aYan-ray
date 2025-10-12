#include <atomic>
#include <chrono>

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

private: // methods:
  static int futex (int* uaddr, int operation, int val,
    std::chrono::milliseconds timeout = std::chrono::milliseconds(-1),
    int* uaddr2 = nullptr, int val3 = 0) noexcept;

  void futex_wait(int expected) noexcept;

  void futex_wake(int count = 1) noexcept;

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
