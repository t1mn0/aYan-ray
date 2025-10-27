#include "Barrier.hpp"
#include "../detail/futex/Futex.hpp"

namespace ayan::sync {

// ------------------------- Barrier Public Methods -------------------------

Barrier::Barrier(size_t count)
  : counter(count), initial_count(count)
{
  if (count == 0) {
    throw std::invalid_argument("[Barrier]: barrier count cannot be zero");
  }
}

void Barrier::arrive_and_wait() {
  size_t gen = generation_counter.load(std::memory_order_acquire);

  // decrease the counter and check if this thread is the last one:
  if (counter.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    // if this thread is the last one:
    counter.store(initial_count, std::memory_order_relaxed);
    generation_counter.fetch_add(1, std::memory_order_release);

    detail::futex_wake(reinterpret_cast<int*>(&generation_counter), INT_MAX);
  }
  else {
    // this thread is not the last one - we are waiting for a generation change:
    while (generation_counter.load(std::memory_order_acquire) == gen) {
      detail::futex_wait(reinterpret_cast<int*>(&generation_counter), gen);
    }
  }
}

} // namespace ayan::sync;
