#pragma once

#include "data/session/session.h"
#include "logic/GameProccess.h"
#include "data/game/manager.h"

class GameProcessManager
{
public:
  void createProcess(Session &session, GameFile gameFile);

  std::vector<logic::GameProcess> getProcesses();
private:
  std::vector<logic::GameProcess> processes;
};