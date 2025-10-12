#include <ayan/sync/mutex/Mutex.hpp>
#include <ayan/sync/detail/futex/futex.hpp>

#include <linux/futex.h>

namespace ayan::sync {

// ------------------------- Mutex Public Methods -------------------------

void Mutex::lock() noexcept {
  // Unlocked -> Locked
  MutexState expected = MutexState::Unlocked;

  if (state.compare_exchange_strong(expected, MutexState::Locked, std::memory_order_acquire)) {
    return; // fast way - successful locked;
  }

  // slow way - mutex is already locked:
  while (true) {
    if (expected == MutexState::Locked || expected == MutexState::Contended) {
      // atomically set `Contended` and check before waiting:
      if (state.compare_exchange_strong(expected, MutexState::Contended, std::memory_order_acquire)) {
          // now we are guaranteed to be waiting on `Contended` state:
          detail::futex_wait(reinterpret_cast<int*>(&state), static_cast<int>(MutexState::Contended));
      }
    }

    // trying to lock the mutex again:
    expected = MutexState::Unlocked;
    if (state.compare_exchange_strong(expected, MutexState::Locked, std::memory_order_acquire)) {
      return;
    }
  }
}

void Mutex::unlock() noexcept {
  // always atomically get the previous state:
  MutexState old_state = state.exchange(MutexState::Unlocked, std::memory_order_release);

  // wake up only if there were threads waiting:
  if (old_state == MutexState::Contended) {
    detail::futex_wake(reinterpret_cast<int*>(&state), 1);
  }
}

} // namespace ayan::sync;
