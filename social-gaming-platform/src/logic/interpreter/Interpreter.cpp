#include "AssignmentRule.h"
#include "Interpreter.h"


namespace logic {
    Interpreter::Interpreter(RuleSpecStack ruleSpecs, Session* session)
        : interpreterState(InterpreterState(std::move(ruleSpecs), session)) {} 


    [[nodiscard]] bool 
    Interpreter::registerRule(RuleType ruleType, std::shared_ptr<IRule> newRule) noexcept {
        auto rule = rulesRegister.find(ruleType);
        if (rule != rulesRegister.end()) {
            return false; // ruleType already has a rule associated to it
        }
        rulesRegister[ruleType] = std::move(newRule);
        return true;
    }


    [[nodiscard]] RuleExecutionOutcome Interpreter::executeRules() noexcept { 
        RuleExecutionOutcome ruleOutcome = RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING;

        while (ruleOutcome != RuleExecutionOutcome::INTERNAL_FAILURE
               && ruleOutcome != RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE
               && ruleOutcome != RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT
               && ruleOutcome != RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT) {
            
            // get the next rule
            auto ruleSpec = interpreterState.getNextRuleSpec();
            if (!ruleSpec) {
                ruleOutcome = RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE;
                break;
            } 
            
            // find the rule in the rule register
            auto rule = rulesRegister.find(ruleSpec.value()->ruleType);
            if (rule == rulesRegister.end()) {
                ruleOutcome = RuleExecutionOutcome::INTERNAL_FAILURE; // rule was not registered
                break;
            }

            ExecuteRuleResult result = rulesRegister[ruleSpec.value()->ruleType]->execute(*ruleSpec, interpreterState);
            ruleOutcome = result.outcome;
            if (ruleOutcome == RuleExecutionOutcome::INTERNAL_FAILURE) {
                break;
            }
            
        }
        interpreterState.setLastOutcome(ruleOutcome);
        return ruleOutcome;
    } // end of executeRule()

    [[nodiscard]] InterpreterState const& Interpreter::getState() const noexcept {
        return interpreterState;
    }
}
