#pragma once

#include <vector>
#include <algorithm>

#include "ProcessTraits.h"

namespace logic
{
  /**
   * @brief A class that schedules processes of a given kind.
   * @param ProcessKind The kind of process to be scheduled. Must have a `ProcessTraits` specialization.
   */
  template <typename ProcessKind>
  class Scheduler
  {
    using Process = ProcessTraits<ProcessKind>;

  private:
    // Processes which are ready to be executed are held here
    std::vector<Process> readyPool;
    // Processes which are waiting for IO operations are held here
    std::vector<Process> ioBoundPool;

    /**
     * @brief Executes a single process for `Scheduler::quantum` ticks. Moves the process to the IO bound pool after an IO operation.
     */
    void executeProcess(Process process);

    /**
     * @brief Returns a lambda that can be used to remove a process from a vector.
     */
    void removeProcess(std::vector<Process> &pool, Process process);

  public:
    /**
     * @brief Adds a process to the ready pool.
     */
    void addProcess(Process process);

    /**
     * @brief Executes all processes in the ready pool in fake parallel.
     * Each process runs until a it's waiting for an IO operation to complete.
     * Clears the IO bound pool of processes that are no longer waiting for IO ticks.
     * If a process is waiting for IO, it is moved to the IO bound pool.
     * If the process is done executing, it is removed from the ready pool.
     */
    void executeInParallel();

    /**
     * @brief Returns whether there are any processes in the ready or IO bound pools.
     */
    bool hasProcesses() const
    {
      return !readyPool.empty() || !ioBoundPool.empty();
    }
  };

  // Implementations

  template <typename ProcessKind>
  void Scheduler<ProcessKind>::addProcess(Process process)
  {
    readyPool.push_back((process));
  }

  template <typename ProcessKind>
  void Scheduler<ProcessKind>::executeInParallel()
  {
    for (Process &process : ioBoundPool)
    {
      if (!process.isWaitingForIO())
      {
        readyPool.push_back(process);

        removeProcess(ioBoundPool, process);
      }
    }

    for (Process &process : readyPool)
    {
      executeProcess(process);
    }
  }

  template <typename ProcessKind>
  void Scheduler<ProcessKind>::executeProcess(Process process)
  {

    if (process.isWaitingForIO())
    {
      ioBoundPool.push_back(process);
      removeProcess(readyPool, process);
      return;
    }

    if (process.isDone())
    {
      removeProcess(readyPool, process);
      return;
    }
    process.tick();
  }

  template <typename ProcessKind>
  void Scheduler<ProcessKind>::removeProcess(std::vector<Process> &pool, Process process)
  {
    pool.erase(std::remove_if(pool.begin(), pool.end(), [&process](Process &p)
                              { return p.getId() == process.getId(); }),
               pool.end());
  }
}
