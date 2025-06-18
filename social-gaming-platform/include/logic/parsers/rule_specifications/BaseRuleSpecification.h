#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "data/data_node.h"
#include "Grammar.h"


namespace logic {
    
    
    /**
     * Base specification required for a rule to execute successfully.
     */
    class BaseRuleSpecification {
    public:
        const RuleType ruleType;
        const int nestedRulesCount;

        BaseRuleSpecification(RuleType ruleType, int nestedRulesCount)
            : ruleType(ruleType), nestedRulesCount(nestedRulesCount) {}

        virtual ~BaseRuleSpecification() = default;

        virtual bool operator==(const BaseRuleSpecification& other) const {
            return ruleType == other.ruleType && 
                   nestedRulesCount == other.nestedRulesCount;
        }
    }; // end of BaseRuleSpecification


    using RuleSpecs = std::vector<std::shared_ptr<BaseRuleSpecification>>;
    using RuleSpecStack = std::stack<std::shared_ptr<BaseRuleSpecification>>;

}