#include "LockGuard.hpp"

namespace ayan::sync {

LockGuard::LockGuard(Mutex& m) noexcept : mutex(m) {
  mutex.lock();
}

LockGuard::~LockGuard() noexcept {
  mutex.unlock();
}

} // namespace ayan::sync;
