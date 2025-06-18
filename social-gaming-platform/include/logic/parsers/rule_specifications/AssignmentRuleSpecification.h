 #pragma once

 #include "rule_specifications/BaseRuleSpecification.h"
 
 namespace logic {

    /**
     * Rule specification for assignment rule.
     */
    class AssignmentRuleSpecification final : public BaseRuleSpecification {
    public:
        const std::string variableName;
        const SocialGamingType variableType;
        const DataNode variableValue;

        AssignmentRuleSpecification(int nestedRulesCount, std::string variableName,
                                    SocialGamingType variableType, DataNode variableValue)
            : BaseRuleSpecification(RuleType::ASSIGNMENT, nestedRulesCount),
            variableName(std::move(variableName)),
            variableType(variableType),
            variableValue(std::move(variableValue)) {}

        bool operator==(const BaseRuleSpecification& other) const override {
            const AssignmentRuleSpecification* derivedOther = dynamic_cast<const AssignmentRuleSpecification*>(&other);
            if (!derivedOther) {
                return false;
            }
            return BaseRuleSpecification::operator==(other)
                && variableName == derivedOther->variableName 
                && variableType == derivedOther->variableType;
                // TODO: compare variableValue
        }

    }; // end of AssignmentRuleSpecification
 }    