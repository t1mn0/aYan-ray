#include "Job.hpp"

#include <cassert>

namespace ayan::jobsys {

void Job::add_dependency(Job* dependent) {
  assert(dependent != nullptr && "Dependent cannot be null");
  assert(dependent != this && "Job cannot depend on itself (cycle!)");

  dependent->pending_dependencies.fetch_add(1, std::memory_order_relaxed);

  Job* old_head = head_dependent.load(std::memory_order_relaxed);

  do {
    dependent->next_dependent = old_head;
  } while (!head_dependent.compare_exchange_weak(
      old_head, dependent,
      std::memory_order_release,   // success: changed;
      std::memory_order_relaxed    // failure: retry;
  ));
}


void Job::set_parent(Job* parent_job) {
  assert(parent_job != nullptr && "Parent cannot be null");
  assert(parent_job != this && "Job cannot be parent of itself");
  assert(parent == nullptr && "Parent already set");

  parent = parent_job;
  parent->unfinished_jobs.fetch_add(1, std::memory_order_relaxed);
}

void Job::execute() {
  assert(function != nullptr && "Job function is null");
  assert(state.load(std::memory_order_acquire) == JobState::EXECUTING);

  function(this, data);
}

void Job::finish() {
  int32_t prev_count = unfinished_jobs.fetch_sub(1, std::memory_order_acq_rel);

  assert(prev_count > 0 && "Double finish() detected!");

  if (prev_count == 1) {
    state.store(JobState::FINISHED, std::memory_order_release);

    if (parent) {
      parent->finish();
    }

    Job* dep = head_dependent.load(std::memory_order_acquire);
    while (dep) {
      Job* next = dep->next_dependent;

      int32_t remaining_deps = dep->pending_dependencies.fetch_sub(1,
        std::memory_order_acq_rel) - 1;

      if (remaining_deps == 0) {
          dep->state.store(JobState::READY, std::memory_order_release);
      }

      dep = next;
    }
  }
}

void Job::reset_to_reuse() {
  assert(state.load(std::memory_order_acquire) == JobState::FINISHED || state.load(std::memory_order_acquire) == JobState::IN_POOL);

  type = JobType::VOID;
  function = nullptr;
  data = nullptr;

  state.store(JobState::IN_POOL, std::memory_order_relaxed);

  head_dependent.store(nullptr, std::memory_order_relaxed);
  next_dependent = nullptr;

  unfinished_jobs.store(1, std::memory_order_relaxed);
  pending_dependencies.store(0, std::memory_order_relaxed);

  parent = nullptr;
}

bool Job::is_finished() const {
  return unfinished_jobs.load(std::memory_order_acquire) == 0;
}

bool Job::has_unfinished_work() const {
  return unfinished_jobs.load(std::memory_order_acquire) > 0;
}

int32_t Job::count_unfinished() const {
  int32_t count = unfinished_jobs.load(std::memory_order_acquire);
  return static_cast<int32_t>(count > 0 ? count : 0);
}

bool Job::is_ready() const {
  return pending_dependencies.load(std::memory_order_acquire) == 0;
}

JobState Job::get_state() const {
  return state.load(std::memory_order_acquire);
}

static_assert(sizeof(Job) <= 128, "Job exceeds cache line size! Consider optimization");

} // namespace ayan::jobsys;
