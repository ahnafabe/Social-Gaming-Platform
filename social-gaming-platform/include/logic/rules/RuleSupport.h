#pragma once

#include "logic/parsers/Grammar.h" // for RuleType


namespace logic {

    /**
     * Outcomes of executing a result. 
     */    
    enum class RuleExecutionOutcome {
        INTERNAL_FAILURE,
        NO_MORE_RULES_TO_EXECUTE,
        SUCCESS_WAITING_FOR_INPUT, // successful rule execution; waiting for player input(s)
        SUCCESS_DELIVERING_OUTPUT, // successful rule execution; waiting for an output to be published
        SUCCESS_WITH_NESTED_RULES_REMAINING,
        SUCCESS_WITH_NO_NESTED_RULES_REMAINING
    }; // end of RuleExecutionOutcome


    /**
     * Results after executing a rule.
     */
    class ExecuteRuleResult {
    public:
        const RuleExecutionOutcome outcome;
        const RuleType ruleType;

        ExecuteRuleResult(RuleExecutionOutcome outcome, RuleType ruleType)
            : outcome(outcome), ruleType(ruleType) {}
    
        bool operator==(const ExecuteRuleResult& other) const {
            return outcome == other.outcome
                && ruleType == other.ruleType;
        }
    }; // end of ExecuteRuleResult

}