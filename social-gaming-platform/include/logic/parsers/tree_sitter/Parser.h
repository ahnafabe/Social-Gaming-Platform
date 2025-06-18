#pragma once

#include <map>
#include <expected>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include <cpp-tree-sitter.h>

#include "logic/BaseRuleSpecification.h"
#include "logic/parsers/IParser2.h"
#include "logic/parsers/tree_sitter/ParsingUtils.h"



extern "C" { TSLanguage *tree_sitter_socialgaming(); }

/**
 * Parser that uses a SocialGaming tree sitter to tokenize
 * the incoming source code.
 */
class Parser {
public:
    
    /**
     * @param sourceCode incoming SocialGamingSourceCode
     * @throws std::invalid_argument in the source code
     */
    Parser(std::string sourceCode);

private:
    ts::Language socialGaming = tree_sitter_socialgaming();
    ts::Parser parser{socialGaming};
    ts::Tree syntaxTree;
    const std::string sourceCode;

    static inline constexpr std::string_view NAME_KEY = "name";
    static inline constexpr std::string_view AUDIENCE_KEY = "has_audience";
    static inline constexpr std::string_view PLAYER_RANGE_KEY = "player_range:";

    /**
     * Parse the source code and return a map of the configuration values.
     */
    [[nodiscard]] std::expected<Configuration, std::string> 
    getConfigurationFromNode(const ts::Node &rootNode) const;
    
    /**
     * Parse the source code and return a map of the gameStateObject values.
     */
    [[nodiscard]] std::expected<GameStateObject, std::string> 
    getGameStateObjectFromNode(const ts::NOde &rootNode, GameStateObjectTypes type) const;


    
};
