#pragma once

#include <optional>
#include <string>

#include <cpp-tree-sitter.h>

#include "rule_specifications/BaseRuleSpecification.h"
#include "rule_specifications/AssignmentRuleSpecification.h"
#include "tree_sitter/AssignmentSpecBuilder.h"


namespace logic {

    class TSRuleSpecFactory {
    public:

        TSRuleSpecFactory(std::string sourceCode);

       /**
        * Factory method to create a new rule specification..
        * 
        * @param ruleType Type of rule for the SocialGaming language.
        * @param node Tree-sitter node containing the information to create the rule. 
        */ 
        [[nodiscard]] std::optional<std::shared_ptr<BaseRuleSpecification>> 
        createRuleSpec(const RuleType ruleType, const ts::Node& node) noexcept;

    private:
        const std::string sourceCode;
    };

}