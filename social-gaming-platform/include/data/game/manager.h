#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <optional>
#include <ranges>

#include "game_file.h"

const std::string GAME_FILE_EXTENSION = ".game";

class GameManager
{
private:
  std::string gameDirectory;
  std::vector<std::string> gameFileNames;

  std::string getGamePath(std::string gameName) const;

public:
  GameManager(std::string gameDirectory) : gameDirectory(gameDirectory)
  {
    refreshGameList();
  };

  std::vector<std::string_view> listGameNames() const;
  std::optional<GameFile> readGameFile(std::string gameName) const;

  void refreshGameList();
};

std::string stringFromFile(std::ifstream &file);