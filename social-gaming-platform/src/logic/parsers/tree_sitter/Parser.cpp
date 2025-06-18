#include "logic/parsers/tree_sitter/Parser.h"

Parser::Parser(std::string sourceCode)
        : sourceCode(sourceCode), 
          parser(socialGaming),
          syntaxTree(parser.parseString(sourceCode))
    {
        if (syntaxTree.hasError()) 
        {
            throw std::invalid_argument(
                std::string("Source code has an error: ")
                + syntaxTree.getRootNode().getSExpr().get()  
            );
        }
    } // end of TreeSitterParser()
    
/* OVERLOADED METHODS */

std::ostream &operator<<(std::ostream &os, const ts::Node &node) {
    os << "Node Type: " << node.getType(); // Adjust this to match your `ts::Node` API
    return os;
}

bool operator==(const ts::Node &lhs, const ts::Node &rhs) {
    return ts_node_eq(lhs.impl, rhs.impl);
}

enum class GameStateObjectTypes {
        Constants,
        Variables,
        PerPlayer,
        PerAudience
    };

std::string nodeToString(const ts::Node &node) const {
    return node.getSourceRange(sourceCode);
}

int parseNumber(const std::string &input) const {
    std::regex numberRegex(R"(-?\d+)");
    std::smatch match;

    if (!std::regex_match(input, match, numberRegex)) {
        throw std::invalid_argument("Invalid number: " + input);
    }
    return std::stoi(input);
}

bool parseBool(const std::string &input) const {
    std::string lowerCaseStr = input;
    std::transform(lowerCaseStr.begin(), lowerCaseStr.end(), lowerCaseStr.begin(), ::tolower);

    if (lowerCaseStr == "true") {
        return true;
    } else if (lowerCaseStr == "false") {
        return false;
    } else {
        throw std::invalid_argument("Invalid boolean: " + input);
    }
}

Parser::getNameFromConfigNode(const ts::Node &configNode) const{
    
}

[[nodiscard]] std::expected<Configuration, std::string> 
Parser::getConfigurationFromNode(const ts::Node &rootNode) const  {
        ts::Node configurationNode = rootNode.getChildByFieldName("configuration");
        if (configurationNode.isNull()) {
            return std::unexpected<std::string>("Error: Configuration node is null or not found.");
        }

        return Configuration(*rootDataNode);
    } // end of parseConfigurationFieldImpl()

[[nodiscard]] std::expected<GameStateObject, std::string> 
GameStateObject getGameStateObjectFromNode(const ts::Node &rootNode, GameStateObjectTypes type) const{

    GameStateObject gameStateObject = create_monostate_node();

    ts::Node gameStateObjectNode;

    switch (type) {
        case GameStateObjectTypes::Constants:
            gameStateObjectNode = rootNode.getChildByFieldName(rootNode, "constants");
            break;
        case GameStateObjectTypes::Variables:
            gameStateObjectNode = rootNode.getChildByFieldName(rootNode, "variables");
            break;
        case GameStateObjectTypes::PerPlayer:
            gameStateObjectNode = rootNode.getChildByFieldName(rootNode, "per_player");
            break;
        case GameStateObjectTypes::PerAudience:
            gameStateObjectNode = rootNode.getChildByFieldName(rootNode, "per_audience");
            break;
        default:
            throw std::invalid_argument("Unknown GameStateObjectType");
            return gameStateObject;
    }

    ts::Node valueMapNode = gameStateObjectNode.getChildByFieldName("map");

    if(valueMapNode.isNull()){
        return std::unexpected<std::string>("Error: No map found in GameStateObject.");
    }

    //parseValueMap();
    return gameStateObject;
}