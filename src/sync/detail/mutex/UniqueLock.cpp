#include <ayan/sync/detail/mutex/UniqueLock.hpp>

#include <stdexcept>

namespace ayan::sync {

UniqueLock::UniqueLock(Mutex& m) noexcept : mutex_ptr(&m) {
  lock();
}

UniqueLock::UniqueLock(UniqueLock&& other) noexcept
  : mutex_ptr(other.mutex_ptr), owns_lock(other.owns_lock)
{
  other.mutex_ptr = nullptr;
  other.owns_lock = false;
}

UniqueLock& UniqueLock::operator=(UniqueLock&& other) noexcept {
  if (this != &other) {
    if (owns_lock) {
      mutex_ptr->unlock();
    }

    mutex_ptr = other.mutex_ptr;
    owns_lock = other.owns_lock;

    other.mutex_ptr = nullptr;
    other.owns_lock = false;
  }
  return *this;
}

UniqueLock::~UniqueLock() noexcept {
  if (owns_lock) {
    mutex_ptr->unlock();
  }
}

void UniqueLock::lock() {
  if (mutex_ptr == nullptr) {
    throw std::runtime_error("[UniqueLock]: mutex is not initialized in the unique_lock object");
  }
  if (owns_lock) {
    throw std::runtime_error("[UniqueLock]: attempting to re-lock the mutex in unique_lock wrapper");
  }

  mutex_ptr->lock();
  owns_lock = true;
}

void UniqueLock::unlock() {
  if (!owns_lock) {
    throw std::runtime_error("[UniqueLock]: attempting to unlock the not locked mutex in unique_lock wrapper");
  }

  mutex_ptr->unlock();
  owns_lock = false;
}

} // namespace ayan::sync;
