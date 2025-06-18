#include "external/GameProcessManager.h"

void GameProcessManager::createProcess(Session &session, GameFile gameFile)
{
  logic::GameProcess newProcess(session, gameFile.getContents());

  processes.push_back(newProcess);
}

std::vector<logic::GameProcess> GameProcessManager::getProcesses()
{
  return processes;
}