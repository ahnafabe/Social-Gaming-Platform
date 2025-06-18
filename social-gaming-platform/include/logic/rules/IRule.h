#pragma once

#include <memory>
#include <vector>

#include "parsers/rule_specifications/BaseRuleSpecification.h"
#include "InterpreterState.h"
#include "RuleSupport.h"


namespace logic {

    /**
     * Rule interface
     */
    class IRule {
    public:
        virtual ~IRule() = default;

        /**
         * Execute the rule.
         * 
         * @param ruleSpec data containing information relevant to the rule.
         * @param interpreterState current state of the interpreter that will be updated by the rule.
         */
        [[nodiscard]] ExecuteRuleResult 
        execute(std::shared_ptr<BaseRuleSpecification> ruleSpec, 
                InterpreterState& interpreterState) noexcept {
            if (ruleSpec->ruleType == RuleType::UNKNOWN) { // unknown rule spec
                return ExecuteRuleResult{RuleExecutionOutcome::INTERNAL_FAILURE, RuleType::UNKNOWN};
            
            // rule spec is expecting more rules than there is in the rule stack
            } else if (interpreterState.ruleSpecsRemaining() < ruleSpec->nestedRulesCount) {
                return ExecuteRuleResult{RuleExecutionOutcome::INTERNAL_FAILURE, RuleType::UNKNOWN};
            }
            return executeImpl(ruleSpec, interpreterState);
        }

    private:
        
        /**
         * Virtual method to be implemented to execute a rule with a given specification.
         * Any nested rules must be pushed back on to the interpreterState ruleSpecStack.
         */
        [[nodiscard]] virtual ExecuteRuleResult 
        executeImpl(std::shared_ptr<BaseRuleSpecification> ruleSpec, 
                    InterpreterState& interpreterState) noexcept = 0;

    }; // end of IRule

}