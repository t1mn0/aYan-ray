#pragma once

#include <atomic>

namespace ayan::jobsys {

enum class JobType : uint8_t {
  RENDER,
  TRANSFORM,
  IO,
  VOID,
};

// ~ Job lifecycle in the program:
enum class JobState : uint8_t {
  IN_POOL,
  PENDING,
  READY,
  EXECUTING,
  FINISHED
};

struct Job {
public: // usings:
  // Stateless lambdas that can be converted to func-pointers:
  using JobFunc = void(*)(Job*, void*);

public: // fields:
  JobType type = JobType::VOID;

  JobFunc function = nullptr;
  void* data = nullptr; // data, that will be passed to function;

  std::atomic<JobState> state = JobState::IN_POOL;

  // ===== ===== ===== Dependency management ===== ===== =====

  // ~ intrusive list:
  // `head_dependent`, like head of intrusive list of Jobs,
  // can be read concurrent by different Job objects:
  std::atomic<Job*> head_dependent = nullptr;

  // next Node in intrusive list of Jobs:
  Job* next_dependent = nullptr;

  // Pending dependency counter, which show how many Jobs must be completed BEFORE it Job starting:
  // +1 for each dependency via add_dependency();
  // When it reaches 0 → Job can be called execute() function:
  std::atomic<uint32_t> pending_dependencies = 0;

  // Work-in-progress counter (children + the Job itself)
  // +1 for each spawned child (inside execute());
  // When it reaches 0 → Job is fully completed:
  std::atomic<uint32_t> unfinished_jobs = 1;

  // Parent for dynamically created child Jobs inside execute()-method;
  // Example: parallel_for creates N subtasks, all have parent = parallel_for_job;
  // When child completes → decrements parent->unfinished_jobs;
  Job* parent = nullptr;

public: // methods:
  Job() = default;
  ~Job() = default;

  // Non-copyable: copying will break the dependency graph and counters:
  Job(const Job&) = delete;
  Job& operator=(const Job&) = delete;

  // Non-movable: Job has pointers from other Jobs (prev/next)
  // moving will make these pointers invalid => :
  Job(Job&&) = delete;
  Job& operator=(Job&&) = delete;

  // We marked, that this Job must be completed BEFORE the dependent is started:
  void add_dependency(Job* dependency);

  // Set parent for spawned tasks (called inside execute());
  // `parent_job` will be the parent Job that is waiting for the child to complete:
  void set_parent(Job* parent);

  void execute();

  // Terminate this Job and notify dependent Jobs;
  // It is called automatically after execute() or from child jobs:
  void finish();

  // Called from the JobPool:
  void reset_to_reuse();

  bool is_finished() const;
  bool has_unfinished_work() const;
  int32_t count_unfinished() const;
  bool is_ready() const;
  JobState get_state() const;
};

} // namespace ayan::jobsys;
