#pragma once

#include <string>

class GameFile
{
private:
  std::string contents;

public:
  GameFile(std::string contents) : contents(contents) {}

  std::string getContents() const
  {
    return contents;
  }
};