#pragma once

namespace logic
{
  using ProcessID = int;

  /**
   * @brief A class that defines the traits of a process.
   * @tparam ProcessKind The kind of process to be defined. Must be specialized.
   */
  template <typename ProcessKind>
  class ProcessTraits
  {
  private:
    ProcessKind process;

  public:
    ProcessTraits(ProcessKind process) : process(process) {};

    /**
     * @brief Returns a uniquely identifying integer for the process.
     */
    ProcessID getId() const = 0;

    /**
     * @brief Executes the process for one tick. Should only perform a single operation.
     */
    void tick() = 0;

    /**
     * @brief Returns whether the process is waiting for an IO operation to complete.
     */
    bool isWaitingForIO() const = 0;

    /**
     * @brief Returns whether the process has completed execution.
     */
    bool isDone() const = 0;
  };
}
