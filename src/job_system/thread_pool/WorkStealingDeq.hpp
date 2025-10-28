#pragma once

#include "../job/Job.hpp"

#include <atomic>
#include <array>
#include <cstdint>
#include <cassert>

namespace ayan::jobsys::thread_pool {

// ThreadLocalDeq is almost lock-free SPMC deque for work-stealing;
// Implemented as Chase-Lev with a fixed size;
//
// Access model:
// - Single Producer (owner): push() and pop() from BOTTOM (LIFO)
// - Multiple Consumers (thieves): steal() from TOP (FIFO)
//
// Guarantees:
// - Lock-free for all operations;
// - ABA-safe via 64-bit indexes;
// - Wait-free for owner (push/pop);
// - Bounded capacity (overflow returns false);

class ThreadLocalDeq {
public: // static constexpr fields:
  static constexpr size_t CAPACITY = 4096;
  static_assert((CAPACITY & (CAPACITY - 1)) == 0, "Capacity must be power of 2");

private: // static constexpr fields:
  // Bitmask for fast modulo (index & MASK):
  static constexpr size_t MASK = CAPACITY - 1;

  // Modified via CAS by thieves (with concurrent stealing)
  // and owner (with race on the last element):
  alignas(64) std::atomic<int64_t> top_ = 0;

  // Only owner can modified (sync between push/pop is not needed)
  alignas(64) std::atomic<int64_t> bottom_ = 0;

  /// Circular buffer для хранения Job*
  alignas(64) std::array<std::atomic<Job*>, CAPACITY> buffer_;

public:
  ThreadLocalDeq(); // setting all default contents as nullptr;
  ~ThreadLocalDeq() = default;

  // Non-copyable, non-movable:
  ThreadLocalDeq(const ThreadLocalDeq&) = delete;
  ThreadLocalDeq& operator=(const ThreadLocalDeq&) = delete;
  ThreadLocalDeq(ThreadLocalDeq&&) = delete;
  ThreadLocalDeq& operator=(ThreadLocalDeq&&) = delete;

  bool push(Job* job);
  Job* pop();

  Job* try_steal();

  size_t capacity();
};

// Invariants:
// 1. At any given time: top ≤ bottom;
// 2. Deq size: size = bottom - top;
// 3. Deq is empty: bottom ≤ top;
// 4. Deq is full: bottom - top ≥ CAPACITY;

// Example of using MASK and circular buffer:
// CAPACITY = 8 => MASK = 7 = 0b111;
// Init: top=0, bottom=0;
// Some operations: push(A), push(B), ..., push(H);
// => top=0, bottom=8 => buffer: [A][B][C][D][E][F][G][H]
//                                ↑                    ↑
//                               top                bottom (last)
// Variants:
// 1. push(): bottom-top+1>=CAPACITY => push() is impossible in this case;
// 2. steal():
//            [A] [B] [C] [D] [E] [F] [G] [H]
//                 ↑                       ↑
//              top=1                   bottom=7
//    (now push() is possible)
// 3. pop():
//            [A] [B] [C] [D] [E] [F] [G] [H]
//             ↑                       ↑
//            top=0                 bottom=6

} // namespace ayan::jobsys::thread_pool
