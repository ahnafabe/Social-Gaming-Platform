#pragma once

#include <map>
#include <expected>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include <cpp-tree-sitter.h>

#include "rule_specifications/BaseRuleSpecification.h"
#include "tree_sitter/ParsingUtils.h"
#include "IParser.h"


namespace logic {

    extern "C" { TSLanguage *tree_sitter_socialgaming(); }

    /**
     * Parser that uses a SocialGaming tree sitter to tokenize
     * the incoming source code.
     */
    class TSParser : public IParser {
    public:
        std::map<std::string, std::string> parseConfigurationSource() const;
        
        /**
         * @param sourceCode incoming SocialGamingSourceCode
         * @throws std::invalid_argument in the source code
         */
        TSParser(std::string sourceCode);
        
        /**
         * Log information about each node in the tree-sitter to the console.
         */
        void printTreeSitterDetails() const noexcept;

    private:
        ts::Language socialGaming = tree_sitter_socialgaming();
        ts::Parser parser{socialGaming};
        ts::Tree syntaxTree;
        const std::string sourceCode;

        static inline constexpr std::string_view NAME_KEY = "name:";
        static inline constexpr std::string_view AUDIENCE_KEY = "audience:";
        static inline constexpr std::string_view PLAYER_RANGE_KEY = "player range:";
        static inline constexpr std::string_view RANGE_KEY = "range:";

        /**
         * Parse the source code and return a map of the configuration values.
         */
        [[nodiscard]] std::expected<Configuration, std::string> 
        parseConfigurationFieldImpl() const noexcept override;
        
        /**
         * Parse the source code and return a map of the global constants values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseGlobalConstantsFieldImpl() const noexcept override;
        /**
         * Parse the source code and return a map of the global variables values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseGlobalVariablesFieldImpl() const noexcept override;

        /**
         * Parse the source code and return a map of the player values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parsePlayerFieldImpl() const noexcept override;

        /**
         * Parse the source code and return a map of the audience values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseAudienceFieldImpl() const noexcept override;
        
        /**
         * Extract the rule specifications from the 
         * source code into a stack.
         */
        [[nodiscard]] std::expected<RuleSpecStack, std::string> 
        parseRuleSpecsImpl() noexcept override;
        

        /* HELPER METHODS NOT FROM IPARSER */

        /**
         * Convert a string to a RuleType (defined in LogicSupport.h)
         * 
         * @param ruleType String representation of what rule to execute.
         */
        [[nodiscard]] std::optional<RuleType> 
        toRuleType(std::string_view ruleType) const noexcept;
        
        /**
         * Recursively log all the children's values.
         * 
         * @param children Iterable tree sitter node.
         */
        void printNamedChildren(ts::Children children) const noexcept;

        /**
         * Parse the source code and return a map of the session values.
         */
        [[nodiscard]] std::expected<DataNode, std::string> 
        traverseTree(const ts::Node &node) const noexcept;

        /**
         * Helper function to parse range values
         */
        [[nodiscard]] std::expected<DataNode, std::string> 
        parseRange(Cursor &cursor) const noexcept;

        /**
         * Helper function to parse list values
         */
        [[nodiscard]] std::expected<DataNode, std::string> 
        parseList(Cursor &cursor) const noexcept;
    };
}