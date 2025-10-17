#include <ayan/sync/condvar/CondVar.hpp>
#include <ayan/sync/mutex/Mutex.hpp>
#include <ayan/sync/detail/futex/Futex.hpp>

namespace ayan::sync {

// ------------------------- CondVar Public Methods -------------------------

void ConditionVar::wait(Mutex& mutex) noexcept {
  waiters_count.fetch_add(1, std::memory_order_relaxed);

  // storing current generation before unlocking mutex:
  uint32_t old_generation = generation_counter.load(std::memory_order_acquire);

  mutex.unlock();

  // wait generation changing:
  while (generation_counter.load(std::memory_order_acquire) == old_generation) {
    detail::futex_wait(reinterpret_cast<int*>(&generation_counter), old_generation);
    // futex_wait may return with EAGAIN (spurious wakeups) - this normal, we continue wait;
  }

  mutex.lock();

  waiters_count.fetch_sub(1, std::memory_order_relaxed);
}

void ConditionVar::notify_one() noexcept {
  if (waiters_count.load(std::memory_order_acquire) > 0) {
    generation_counter.fetch_add(1, std::memory_order_release);
    detail::futex_wake(reinterpret_cast<int*>(&generation_counter), 1);
  }
}

void ConditionVar::notify_all() noexcept {
  if (waiters_count.load(std::memory_order_acquire) > 0) {
    generation_counter.fetch_add(1, std::memory_order_release);
    detail::futex_wake(reinterpret_cast<int*>(&generation_counter), INT_MAX);
  }
}

} // namespace ayan::sync;
