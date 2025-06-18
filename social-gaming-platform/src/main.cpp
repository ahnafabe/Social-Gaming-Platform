#include <iostream>
#include <fstream>
#include <vector>

#include "data/data.h"
#include "logic/parsers/tree_sitter/TSParser.h"



int main()
{
    std::string sourceCode;
    
    std::string filePath = "../games/RPS.game";
    std::ifstream file(filePath);
      if (file.is_open())
    {
      std::string line;
      while (getline(file, line))
      {
        sourceCode += line + "\n";
      }
      file.close();
    }
    else{
      std::cerr << "Unable to open file: " << filePath << std::endl;
      return 1;
    }
    if (sourceCode.empty())
    {
      std::cerr << "File is empty: " << filePath << std::endl;
      return 1;
    }
    logic::TSParser parser(sourceCode);

    auto configuration = parser.parseConfigurationField();
    if (!configuration) {
        std::cerr << "Error: " << configuration.error() << std::endl;
        return 1;
    }

    auto globalConstants = parser.parseGlobalConstantsField();
    if (!globalConstants) {
        std::cerr << "Error: " << globalConstants.error() << std::endl;
        return 1;
    }

    auto globalVariables = parser.parseGlobalVariablesField();
    if (!globalVariables) {
        std::cerr << "Error: " << globalVariables.error() << std::endl;
        return 1;
    }

    auto player = parser.parsePlayerField();
    if (!player) {
        std::cerr << "Error: " << player.error() << std::endl;
        return 1;
    }

    auto audience = parser.parseAudienceField();
    if (!audience) {
        std::cerr << "Error: " << audience.error() << std::endl;
        return 1;
    }

    return 0;
}
