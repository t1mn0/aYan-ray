#pragma once

#include <atomic>

namespace ayan::tasksystem {

enum class TaskType : uint8_t {
  RENDER,
  TRANSFORM,
  IO,
  VOID,
};

// ~ Task lifecycle in the program:
enum class TaskState : uint8_t {
  IN_POOL,
  PENDING,
  READY,
  EXECUTING,
  FINISHED
};

struct Task {
public: // usings:
  // Stateless lambdas that can be converted to func-pointers:
  using TaskFunc = void(*)(Task*, void*);

public: // fields:
  TaskType type = TaskType::VOID;

  TaskFunc function = nullptr;
  void* data = nullptr; // data, that will be passed to function;

  std::atomic<TaskState> state = TaskState::IN_POOL;

  // ===== ===== ===== Dependency management ===== ===== =====

  // ~ intrusive list:
  // `head_dependent`, like head of intrusive list of Tasks,
  // can be read concurrent by different Task objects:
  std::atomic<Task*> head_dependent = nullptr;

  // next Node in intrusive list of Tasks:
  Task* next_dependent = nullptr;

  // Pending dependency counter, which show how many Tasks must be completed BEFORE it Task starting:
  // +1 for each dependency via add_dependency();
  // When it reaches 0 → Task can be called execute() function:
  std::atomic<uint32_t> pending_dependencies = 0;

  // Work-in-progress counter (children + the Task itself)
  // +1 for each spawned child (inside execute());
  // When it reaches 0 → Task is fully completed:
  std::atomic<uint32_t> unfinished_tasks = 1;

  // Parent for dynamically created child Tasks inside execute()-method;
  // Example: parallel_for creates N subtasks, all have parent = parallel_for_task;
  // When child completes → decrements parent->unfinished_tasks;
  Task* parent = nullptr;

public: // methods:
  Task() = default;
  ~Task() = default;

  // Non-copyable: copying will break the dependency graph and counters:
  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  // Non-movable: Task has pointers from other Tasks (prev/next)
  // moving will make these pointers invalid => :
  Task(Task&&) = delete;
  Task& operator=(Task&&) = delete;

  // We marked, that this Task must be completed BEFORE the dependent is started:
  void add_dependency(Task* dependency);

  // Set parent for spawned tasks (called inside execute());
  // `parent_task` will be the parent Task that is waiting for the child to complete:
  void set_parent(Task* parent);

  void execute();

  // Terminate this Task and notify dependent Tasks;
  // It is called automatically after execute() or from child tasks:
  void finish();

  // Called from the TaskPool:
  void reset_to_reuse();

  bool is_finished() const;
  bool has_unfinished_work() const;
  int32_t count_unfinished() const;
  bool is_ready() const;
  TaskState get_state() const;
};

} // namespace ayan::tasksystem;
