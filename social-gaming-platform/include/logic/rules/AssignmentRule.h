#pragma once

#include "parsers/rule_specifications/AssignmentRuleSpecification.h"
#include "IRule.h"


namespace logic { 

  class AssignmentRule final : public IRule {
    private:
        [[nodiscard]] ExecuteRuleResult
        executeImpl(std::shared_ptr<BaseRuleSpecification> ruleSpec,
                    InterpreterState& interpreterState) noexcept override;

    }; // end of AssignmentRule

}
