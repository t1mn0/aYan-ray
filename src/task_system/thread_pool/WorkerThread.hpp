#pragma once

#include "../task/Task.hpp"
#include "ThreadLocalDeq.hpp"
#include "../../sync/mutex/Mutex.hpp"
#include "../../sync/condvar/CondVar.hpp"

#include <thread>
#include <atomic>
#include <random>
#include <chrono>

namespace ayan::tasksystem::thread_pool {

// Lifecycle:
// 1. Creating and launching thread
// 2. Worker loop: get_task() → execute() → finish()
// 3. Graceful shutdown during destruction
//
// Strategy for getting Task units:
// 1. Pop from a local deque (LIFO, cache-friendly)
// 2. Steal from random victims (FIFO, load balancing)
// 3. Sleep if there is no work (energy efficiency)
class WorkerThread {
public: // substructs:
  struct StealingStrategy {
    uint8_t max_steal_attempts{4};
    uint8_t max_random_victims{2};
    // sleep-time if there is no work:
    std::chrono::milliseconds sleep_time{1};
  };

  // TODO for profiling: struct Statistics {};

private: // fields:
  // In ThreadPool it is index in vector of workers:
  uint8_t worker_id;

  // ThreadPool* pool; // for stealing;
  ThreadLocalDeq task_deq;
  std::thread thread;

  std::atomic<bool> should_stop{false};
  std::atomic<bool> is_sleeping{false};

  sync::Mutex sleep_mutex_;
  sync::ConditionVar time_to_wake;

  StealingStrategy stealing_strategy;

  // Random generator to select random victims for theft:
  std::mt19937 rng_;

public: // methods:
  WorkerThread(uint32_t worker_id);
  ~WorkerThread();

  // Non-copyable, non-movable:
  WorkerThread(const WorkerThread&) = delete;
  WorkerThread& operator=(const WorkerThread&) = delete;
  WorkerThread(WorkerThread&&) = delete;
  WorkerThread& operator=(WorkerThread&&) = delete;

  void start();
  void async_stop();

  bool is_running() const noexcept;

  bool try_submit(Task* task);

  void wake_if_sleeping();

  [[nodiscard]] Task* try_steal();

private:
  void worker_loop();
  [[nodiscard]] Task* get_task();
  void execute_task(Task* task); // should be noexcept;

  [[nodiscard]] WorkerThread* select_random_victim();

  void sleep_if_no_work();
};

} // namespace ayan::tasksystem::thread_pool
