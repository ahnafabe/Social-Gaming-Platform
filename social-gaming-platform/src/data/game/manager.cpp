#include "data/game/manager.h"

using namespace std::filesystem;

std::vector<std::string_view> GameManager::listGameNames() const
{
  std::vector<std::string_view> gameNames;

  for (const auto &gameFileName : gameFileNames)
  {
    gameNames.push_back(std::string_view(gameFileName).substr(0, gameFileName.size() - GAME_FILE_EXTENSION.size()));
  }

  return gameNames;
}

std::optional<GameFile> GameManager::readGameFile(std::string gameName) const
{
  std::ifstream file(getGamePath(gameName));

  if (!file)
  {
    return std::nullopt;
  }

  GameFile gameFile = GameFile(stringFromFile(file));

  return gameFile;
}

void GameManager::refreshGameList()
{
    gameFileNames.clear();

    if (!std::filesystem::exists(gameDirectory)) {
        std::cerr << "Error: Game directory does not exist: " << gameDirectory << std::endl;
        //create directory
        std::filesystem::create_directories(gameDirectory);
    }

    for (std::filesystem::recursive_directory_iterator i(gameDirectory), end; i != end; ++i)
    {
        if (!std::filesystem::is_directory(i->path()) && i->path().extension() == GAME_FILE_EXTENSION)
        {
            gameFileNames.push_back(i->path().filename().string());
        }
    }
}

std::string GameManager::getGamePath(std::string gameName) const
{
  return gameDirectory + "/" + gameName + GAME_FILE_EXTENSION;
}

// Credit to Jerry on StackOverflow
// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::string stringFromFile(std::ifstream &file)
{
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  std::string buffer(size, ' ');
  file.seekg(0);
  file.read(&buffer[0], size);

  return buffer;
}