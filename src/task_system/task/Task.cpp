#include "Task.hpp"

#include <cassert>

namespace ayan::tasksystem {

void Task::add_dependency(Task* dependent) {
  assert(dependent != nullptr && "Dependent cannot be null");
  assert(dependent != this && "Task cannot depend on itself (cycle!)");

  dependent->pending_dependencies.fetch_add(1, std::memory_order_relaxed);

  Task* old_head = head_dependent.load(std::memory_order_relaxed);

  do {
    dependent->next_dependent = old_head;
  } while (!head_dependent.compare_exchange_weak(
      old_head, dependent,
      std::memory_order_release,   // success: changed;
      std::memory_order_relaxed    // failure: retry;
  ));
}


void Task::set_parent(Task* parent_job) {
  assert(parent_job != nullptr && "Parent cannot be null");
  assert(parent_job != this && "Task cannot be parent of itself");
  assert(parent == nullptr && "Parent already set");

  parent = parent_job;
  parent->unfinished_tasks.fetch_add(1, std::memory_order_relaxed);
}

void Task::execute() {
  assert(function != nullptr && "Task function is null");
  assert(state.load(std::memory_order_acquire) == TaskState::EXECUTING);

  function(this, data);
}

void Task::finish() {
  uint32_t prev_count = unfinished_tasks.fetch_sub(1, std::memory_order_acq_rel);

  assert(prev_count > 0 && "Double finish() detected!");

  if (prev_count == 1) {
    state.store(TaskState::FINISHED, std::memory_order_release);

    if (parent) {
      parent->finish();
    }

    Task* dep = head_dependent.load(std::memory_order_acquire);
    while (dep) {
      Task* next = dep->next_dependent;

      uint32_t remaining_deps = dep->pending_dependencies.fetch_sub(1,
        std::memory_order_acq_rel) - 1;

      if (remaining_deps == 0) {
          dep->state.store(TaskState::READY, std::memory_order_release);
      }

      dep = next;
    }
  }
}

void Task::reset_to_reuse() {
  assert(state.load(std::memory_order_acquire) == TaskState::FINISHED || state.load(std::memory_order_acquire) == TaskState::IN_POOL);

  type = TaskType::VOID;
  function = nullptr;
  data = nullptr;

  state.store(TaskState::IN_POOL, std::memory_order_relaxed);

  head_dependent.store(nullptr, std::memory_order_relaxed);
  next_dependent = nullptr;

  unfinished_tasks.store(1, std::memory_order_relaxed);
  pending_dependencies.store(0, std::memory_order_relaxed);

  parent = nullptr;
}

bool Task::is_finished() const {
  return unfinished_tasks.load(std::memory_order_acquire) == 0;
}

bool Task::has_unfinished_work() const {
  return unfinished_tasks.load(std::memory_order_acquire) > 0;
}

int32_t Task::count_unfinished() const {
  uint32_t count = unfinished_tasks.load(std::memory_order_acquire);
  return static_cast<uint32_t>(count > 0 ? count : 0);
}

bool Task::is_ready() const {
  return pending_dependencies.load(std::memory_order_acquire) == 0;
}

TaskState Task::get_state() const {
  return state.load(std::memory_order_acquire);
}

static_assert(sizeof(Task) <= 128, "Task exceeds cache line size! Consider optimization");

} // namespace ayan::tasksystem;
