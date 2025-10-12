#include <ayan/sync/mutex/Mutex.hpp>

#include <linux/futex.h>

namespace ayan::sync {

// ------------------------- Mutex Private Methods -------------------------

// ----- ----- ----- ----- ----- ----- ----- ----- -----
// Mutex.futex(args);                                  |
// ----- ----- ----- ----- ----- ----- ----- ----- -----
// Method args:                                        |
// - uaddr: pointer to an atomic var                   |
// - operation: FUTEX_WAIT or FUTEX_WAKE in our case   |
// - val:                                              |
//   - for FUTEX_WAIT: expected value of the variable  |
//   - for FUTEX_WAKE: number of threads to wake up    |
// - timeout: maximum waiting time                     |
// - other args are for more complex operations        |
// (not used in the base mutex)                        |
// ----- ----- ----- ----- ----- ----- ----- ----- -----
int Mutex::futex(int* uaddr, int operation, int val,
  std::chrono::milliseconds timeout, int* uaddr2, int val3) noexcept
{
  // endless waiting:
  if (timeout.count() < 0) {
    return syscall(SYS_futex, uaddr, operation, val, nullptr, uaddr2, val3);
  }

  // finite waiting:
  auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout);

  // timeout has expired:
  if (total_ns.count() <= 0) {
    return -1;
  }

  // timespec is a standard POSIX structure for representing time with nanosecond precision;
  struct timespec ts;
  ts.tv_sec = total_ns.count() / 1'000'000'000;
  ts.tv_nsec = total_ns.count() % 1'000'000'000;

  return syscall(SYS_futex, uaddr, operation, val, &ts, uaddr2, val3);
}

// FUTEX_PRIVATE_FLAG flag allows to avoid interprocess synchronization:
void Mutex::futex_wait(int expected) noexcept {
  futex(reinterpret_cast<int*>(&state), FUTEX_WAIT | FUTEX_PRIVATE_FLAG, expected);
}

void Mutex::futex_wake(int count) noexcept {
  futex(reinterpret_cast<int*>(&state), FUTEX_WAKE | FUTEX_PRIVATE_FLAG, count);
}

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
          futex_wait(static_cast<int>(MutexState::Contended));
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
    futex_wake(1);
  }
}

} // namespace ayan;
