#include "tree_sitter/TSRuleSpecFactory.h"

namespace logic {

    TSRuleSpecFactory::TSRuleSpecFactory(std::string sourceCode)
    : sourceCode(sourceCode) {}


    [[nodiscard]] std::optional<std::shared_ptr<BaseRuleSpecification>> 
    TSRuleSpecFactory::createRuleSpec(const RuleType ruleType, 
                                  const ts::Node& node) noexcept {
        switch(ruleType) {
            case RuleType::ASSIGNMENT:
                AssignmentSpecBuilder builder = AssignmentSpecBuilder();
                return builder.createSpec(sourceCode, node);

            // TODO: LOGIC-14: Add support for MESSAGE
            // TODO: LOGIC-15: Add support for SCORES
            
            // TODO: LOGIC-16: Add support for INPUT [CHOICE]
            // TODO: LOGIC-17: Add support for INPUT [TEXT]
            // TODO: LOGIC-18: Add support for INPUT [RANGE]
            // TODO: LOGIC-19: Add support for INPUT [VOTE]

            // TODO: LOGIC-20: Add support for FOR-LOOP
            // TODO: LOGIC-21: Add support for WHILE-LOOP
            // TODO: LOGIC-22: Add support for PARALLEL-FOR-LOOP
            // TODO: LOGIC-23: Add support for IN-PARALLEL
            // TODO: LOGIC-24: Add support for MATCH
        }            
        return std::nullopt;
    } // end of createRule()
    
}