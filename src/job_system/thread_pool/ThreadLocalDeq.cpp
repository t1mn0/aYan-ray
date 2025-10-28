#include "ThreadLocalDeq.hpp"

namespace ayan::jobsys::thread_pool {

ThreadLocalDeq::ThreadLocalDeq() {
  for (auto& slot : buffer_) {
    slot.store(nullptr, std::memory_order_relaxed);
  }
}

bool ThreadLocalDeq::try_push(Job* job) {
  assert(job != nullptr && "Cannot push nullptr Job");

  int64_t b = bottom_.load(std::memory_order_relaxed);
  int64_t t = top_.load(std::memory_order_acquire); // using acquire <=> most recent top value;

  if (b - t >= static_cast<int64_t>(CAPACITY)) {
    // Deq is full;
    return false;
  }

  // In this method main feature is release-fence.
  // It creates a happens-before relationship:

  // Relaxed: release below will provide visibility;
  // Until we increase bottom_, thieves won't see this memory cell:
  buffer_[b & MASK].store(job, std::memory_order_relaxed);

  // Release-fence: ensures that the buffer entry is visible BEFORE the bottom is increased;
  // This is critical for steal() - the thief will see the correct Job:
  std::atomic_thread_fence(std::memory_order_release);

  // Relaxed OK: fence выше обеспечивает ordering
  bottom_.store(b + 1, std::memory_order_relaxed);

  return true;
}

Job* ThreadLocalDeq::try_pop() {
  // local change, other threads may not see it:
  int64_t b = bottom_.load(std::memory_order_relaxed) - 1;
  bottom_.store(b, std::memory_order_relaxed);

  // SeqCst -fence() for synchronization with steal():
  // Possible scenarios:
  // 1. -fence() was executed BEFORE steal() → steal() will see new bottom;
  // 2. -fence() was executed AFTER steal() → this thread will see the result of steal():
  // Guarantees total order between pop() and competitive steal():
  std::atomic_thread_fence(std::memory_order_seq_cst);

  int64_t t = top_.load(std::memory_order_relaxed);

  Job* job = nullptr;

  if (t <= b) {
    job = buffer_[b & MASK].load(std::memory_order_relaxed);

    if (t == b) {
      // If it was last item, race with steal() is possible;
      // Trying atomicity get it through CAS(top):

      // Race scenario:
      // If owner (pop()) seeing, that t==b, he will try CAS(top),
      // and if thief want to steal last item in deq, he will try CAS(top) and do t → t+1;
      // Main feature: seq_cst guarantees that only one CAS'll be in time;
      // And therefore, it is necessary to provide fallbacks in case the CAS fails for both cases;

      if (!top_.compare_exchange_strong(
        t, t + 1,
        std::memory_order_seq_cst,   // Success: full sync;
        std::memory_order_relaxed))  // Failure: thief stole;
      {
        // (fallback for CAS operation):
        // It race was losed (thief stole last item):
        job = nullptr;
      }

      // Restoring the bottom (deque is empty now):
      bottom_.store(b + 1, std::memory_order_relaxed);
    }
  }
  else {
    // Deque was empty (t > b) => restoring the bottom:
    bottom_.store(b + 1, std::memory_order_relaxed);
  }

  return job;
}

Job* ThreadLocalDeq::try_steal() {
  // Read top_ with acquire (sync with previous succesful steal()/pop());
  // If another thief increased top_, we are guaranteed to see this change:
  int64_t t = top_.load(std::memory_order_acquire);

  // seq_cst guarantees total order between owner [pop()] and thief (us) [steal()]
  // and that we see is the last published top_ value:
  std::atomic_thread_fence(std::memory_order_seq_cst);

  // Read bottom_ from acquire (thief can see push() from owner);
  // Also, release-fence in [push()] + this acquire create happens-before;
  int64_t b = bottom_.load(std::memory_order_acquire);

  if (t >= b) {
    // Deque empty;
    return nullptr;
  }

  // Relaxed OK: acquire with bottom_/top_ guarantees visibility;
  Job* job = buffer_[t & MASK].load(std::memory_order_relaxed);

  // We are trying to atomically catch Job* via CAS on top_;
  // Competing with other thieves and with the owner (if this is the last item):
  if (!top_.compare_exchange_strong(
      t, t + 1,
      std::memory_order_seq_cst,   // Success: full sync;
      std::memory_order_relaxed))  // Failure: another thief or owner was first;
  {
    // If thief lose race:
    return nullptr;
  }

  // Succesfully stole:
  return job;
}

constexpr size_t ThreadLocalDeq::capacity() {
  return CAPACITY;
}

} // namespace ayan::jobsys::thread_pool;
