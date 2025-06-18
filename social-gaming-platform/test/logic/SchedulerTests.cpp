#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "logic/scheduler/ProcessTraits.h"
#include "logic/scheduler/Scheduler.h"

class FakeProcess
{
private:
  int totalTicks;
  std::vector<int> ioOperationTicks;
  logic::ProcessID id;

public:
  FakeProcess(logic::ProcessID id, int totalTicks, std::vector<int> ioOperationTicks)
      : id(id), totalTicks(totalTicks), ioOperationTicks(ioOperationTicks) {}

  void tick()
  {
    totalTicks--;
  }

  bool isDone() const
  {
    return totalTicks == 0;
  }

  logic::ProcessID getId() const
  {
    return id;
  }

  bool isCurrentOperationIO() const
  {
    return !ioOperationTicks.empty() && ioOperationTicks.back() == totalTicks - 1;
  }
};

template <>
class logic::ProcessTraits<FakeProcess>
{
private:
  FakeProcess *process;

public:
  ProcessTraits(FakeProcess* process) : process(process) {};

  void tick()
  {
    process->tick();
  }

  bool isWaitingForIO() const
  {
    return process->isCurrentOperationIO();
  }

  bool isDone() const
  {
    return process->isDone();
  }

  logic::ProcessID getId() const
  {
    return process->getId();
  }
};

/**
 *
 * TESTS START HERE
 *
 */

TEST(SchedulerTests, ExecutesAProcessToCompletion)
{
  FakeProcess process (0, 23, {});
  logic::Scheduler<FakeProcess> scheduler = logic::Scheduler<FakeProcess>(); 
  scheduler.addProcess(logic::ProcessTraits<FakeProcess>(&process));

  while (scheduler.hasProcesses())
  {
    scheduler.executeInParallel();
  }

  EXPECT_TRUE(process.isDone());
};

TEST(SchedulerTests, ExecutesMultipleProcessesInParallel)
{
  FakeProcess process1 = FakeProcess(0, 23, {});
  FakeProcess process2 = FakeProcess(1, 4, {});
  FakeProcess process3 = FakeProcess(2, 30, {});
  FakeProcess process4 = FakeProcess(3, 2, {});

  logic::Scheduler<FakeProcess> scheduler = logic::Scheduler<FakeProcess>();

  scheduler.addProcess(logic::ProcessTraits<FakeProcess>(&process1));
  scheduler.addProcess(logic::ProcessTraits<FakeProcess>(&process2));
  scheduler.addProcess(logic::ProcessTraits<FakeProcess>(&process3));
  scheduler.addProcess(logic::ProcessTraits<FakeProcess>(&process4));

  // tick 0
  EXPECT_FALSE(process1.isDone());
  EXPECT_FALSE(process2.isDone());
  EXPECT_FALSE(process3.isDone());
  EXPECT_FALSE(process4.isDone());
  
  scheduler.executeInParallel();

  // tick 1
  EXPECT_FALSE(process1.isDone());
  EXPECT_FALSE(process2.isDone());
  EXPECT_FALSE(process3.isDone());
  EXPECT_FALSE(process4.isDone());
  
  scheduler.executeInParallel();

  // tick 2; process 4 is done
  EXPECT_FALSE(process1.isDone());
  EXPECT_FALSE(process2.isDone());
  EXPECT_FALSE(process3.isDone());
  EXPECT_TRUE(process4.isDone());
  
  scheduler.executeInParallel();
  scheduler.executeInParallel();
  
  // tick 4; process 2 & 4 are done
  EXPECT_FALSE(process1.isDone());
  EXPECT_TRUE(process2.isDone());
  EXPECT_FALSE(process3.isDone());
  EXPECT_TRUE(process4.isDone());
  
  while (scheduler.hasProcesses())
  {
    scheduler.executeInParallel();
  }

  EXPECT_TRUE(process1.isDone());
  EXPECT_TRUE(process3.isDone());
};

// TEST(SchedulerTests, ExeuctesAProcessWithAnIOOperation)
// {
//   FakeProcess process = FakeProcess(0, 6, {3});

//   logic::Scheduler<FakeProcess> scheduler = logic::Scheduler<FakeProcess>();
//   auto traits = logic::ProcessTraits(&process);

//   scheduler.addProcess(&traits);

//   scheduler.executeInParallel();
//   EXPECT_TRUE(scheduler.hasProcesses());
//   EXPECT_FALSE(process.isDone());

//   scheduler.executeInParallel();
//   EXPECT_TRUE(process.isDone());
// };
