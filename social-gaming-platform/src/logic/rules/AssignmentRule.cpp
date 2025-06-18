#include "AssignmentRule.h"


namespace logic {

    [[nodiscard]] ExecuteRuleResult
    AssignmentRule::executeImpl(std::shared_ptr<BaseRuleSpecification> ruleSpec,
                                InterpreterState& interpreterState) noexcept {
        // TODO: LOGIC-10
        return ExecuteRuleResult {
            RuleExecutionOutcome::INTERNAL_FAILURE,
            RuleType::ASSIGNMENT
        };
    } // end of executeImpl()

}