#pragma once

#include <expected>
#include <string>
#include <regex>
#include <iostream>

#include "rule_specifications/BaseRuleSpecification.h"
#include "data/session/session.h"


namespace logic {

    /**
     * Parser interface
     */
    class IParser {
    public:
        virtual ~IParser() = default;

        /**
         * Parse the source code and return a map of the configuration values.
         */
        [[nodiscard]] std::expected<Configuration, std::string> 
        parseConfigurationField() const noexcept {
            return parseConfigurationFieldImpl();
        }
        
        /**
         * Parse the source code and return a map of the global constants values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseGlobalConstantsField() const noexcept {
            return parseGlobalConstantsFieldImpl();
        }

        /**
         * Parse the source code and return a map of the global variables values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseGlobalVariablesField() const noexcept {
            return parseGlobalVariablesFieldImpl();
        }
        
        /**
         * Parse the source code and return a map of the player values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parsePlayerField() const noexcept {
            return parsePlayerFieldImpl();
        }
        
        /**
         * Parse the source code and return a map of the audience values.
         */
        [[nodiscard]] std::expected<GameStateObject, std::string> 
        parseAudienceField() const noexcept {
            return parseAudienceFieldImpl();
        }

        /**
         * Parse the source code and return stack of rule specifications.
         */
        [[nodiscard]] std::expected<RuleSpecStack, std::string> 
        parseRuleSpecs() {
            return parseRuleSpecsImpl();
        }
    
    private:
        [[nodiscard]] virtual std::expected<Configuration, std::string> 
        parseConfigurationFieldImpl() const noexcept = 0;
        
        [[nodiscard]] virtual std::expected<GameStateObject, std::string> 
        parseGlobalConstantsFieldImpl() const noexcept = 0;
        
        [[nodiscard]]virtual std::expected<GameStateObject, std::string> 
        parseGlobalVariablesFieldImpl() const noexcept = 0;
        
       [[nodiscard]] virtual std::expected<GameStateObject, std::string> 
        parsePlayerFieldImpl() const noexcept = 0;
        
        [[nodiscard]] virtual std::expected<GameStateObject, std::string> 
        parseAudienceFieldImpl() const noexcept = 0;
        
        [[nodiscard]] virtual std::expected<RuleSpecStack, std::string> 
        parseRuleSpecsImpl() noexcept = 0;

    };

}