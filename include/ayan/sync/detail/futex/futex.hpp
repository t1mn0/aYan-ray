#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <chrono>

namespace ayan::sync::detail {

// ----- ----- ----- ----- ----- ----- ----- ----- -----
//        KEY EFFECTIVE SYNC MECHANISM : FUTEX()       |
// ----- ----- ----- ----- ----- ----- ----- ----- -----
//                     futex(args);                    |
// ----- ----- ----- ----- ----- ----- ----- ----- -----
//                       args:                         |
// - uaddr: pointer to an atomic var                   |
// - operation: FUTEX_WAIT or FUTEX_WAKE in our case   |
// - val:                                              |
//   - for FUTEX_WAIT: expected value of the variable  |
//   - for FUTEX_WAKE: number of threads to wake up    |
// - timeout: maximum waiting time                     |
// - other args are for more complex operations        |
// (not used in this project)                          |
// ----- ----- ----- ----- ----- ----- ----- ----- -----
inline int futex(int* uaddr, int operation, int val,
  std::chrono::milliseconds timeout = std::chrono::milliseconds(-1),
  int* uaddr2 = nullptr, int val3 = 0) noexcept
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

  // `timespec` is a standard POSIX structure for representing time with nanosecond precision;
  struct timespec ts;
  ts.tv_sec = total_ns.count() / 1'000'000'000;
  ts.tv_nsec = total_ns.count() % 1'000'000'000;

  return syscall(SYS_futex, uaddr, operation, val, &ts, uaddr2, val3);
}

// FUTEX_PRIVATE_FLAG allows to avoid interprocess synchronization:
inline void futex_wait(int* uaddr, int expected) noexcept {
  futex(uaddr, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, expected);
}

inline void futex_wake(int* uaddr, int count) noexcept {
  futex(uaddr, FUTEX_WAKE | FUTEX_PRIVATE_FLAG, count);
}

} // namespace ayan::sync::detail;
