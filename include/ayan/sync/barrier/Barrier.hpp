#include <atomic>
#include <cstddef>

namespace ayan::sync {

class Barrier {
private: // fields:
  std::atomic<size_t> counter;
  const size_t initial_count;
  std::atomic<size_t> generation_counter = 0;

public: // methods:
  explicit Barrier(size_t count);
  Barrier(const Barrier&) = delete;
  Barrier& operator=(const Barrier&) = delete;

  void arrive_and_wait();
}; // class Barrier;

} // namespace ayan::sync;
