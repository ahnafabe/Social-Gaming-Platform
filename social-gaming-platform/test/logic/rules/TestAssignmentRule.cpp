#include <gtest/gtest.h>

#include "logic/rules/AssignmentRule.h"


namespace TestAssignmentRule {
    struct MockDataNode : DataNode {};
}


// TODO: LOGIC-10
TEST(TestAssignmentRule, ExecuteRuleFail) {

    logic::AssignmentRule rule{};
    auto ruleSpec = std::make_shared<logic::AssignmentRuleSpecification> (0, 
                                                                          std::string("a"), 
                                                                          logic::SocialGamingType::STRING, 
                                                                          TestAssignmentRule::MockDataNode());

    logic::RuleSpecStack ruleSpecStack;
    Session session;
    logic::InterpreterState state (ruleSpecStack, &session);

    logic::ExecuteRuleResult result = rule.execute(ruleSpec, state);

    EXPECT_EQ(result, logic::ExecuteRuleResult(logic::RuleExecutionOutcome::INTERNAL_FAILURE, 
                                               logic::RuleType::ASSIGNMENT));
};