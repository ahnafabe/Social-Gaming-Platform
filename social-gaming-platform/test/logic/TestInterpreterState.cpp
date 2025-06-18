#include <gtest/gtest.h>

#include "logic/interpreter/InterpreterState.h"
#include "logic/rules/IRule.h"


namespace TestInterpreterState {

    class MockRuleSpecification final : public logic::BaseRuleSpecification {
    public:

        MockRuleSpecification(int nestedRulesCount)
            : logic::BaseRuleSpecification(logic::RuleType::ASSIGNMENT, nestedRulesCount) {}

        bool operator==(const logic::BaseRuleSpecification& other) const override {
            const MockRuleSpecification* derivedOther = dynamic_cast<const MockRuleSpecification*>(&other);
            if (!derivedOther) {
                return false;
            }
            return BaseRuleSpecification::operator==(other);
        }

    }; // end of MockRuleSpecification
    

    class MockRule final : public logic::IRule {
    private:
        [[nodiscard]] logic::ExecuteRuleResult
        executeImpl(std::shared_ptr<logic::BaseRuleSpecification> ruleSpec,
                    logic::InterpreterState& interpreterState) noexcept override {
            return logic::ExecuteRuleResult {
                logic::RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING,
                logic::RuleType::ASSIGNMENT
            };
        } // end of MockRule
    };
}


TEST(TestInterpreterState, EmptyRuleSpecStack) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;

    logic::InterpreterState state (std::move(ruleSpecStack), &session);

    EXPECT_EQ(state.ruleSpecsRemaining(), 0);
    EXPECT_EQ(state.getNextRuleSpec(), std::nullopt);

};


TEST(TestInterpreterState, RuleSpecStackModifiers) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;
    ruleSpecStack.push(std::make_shared<TestInterpreterState::MockRuleSpecification>(0));

    logic::InterpreterState state (std::move(ruleSpecStack), &session);

    EXPECT_EQ(state.ruleSpecsRemaining(), 1);
    
    state.addRuleSpec(std::make_shared<TestInterpreterState::MockRuleSpecification>(1));
    EXPECT_EQ(state.ruleSpecsRemaining(), 2);

    // validate stack access (FIFO)

    auto spec = state.getNextRuleSpec();
    EXPECT_NE(spec, std::nullopt);
    EXPECT_EQ(state.ruleSpecsRemaining(), 1);
    const auto* actualSpec = dynamic_cast<const TestInterpreterState::MockRuleSpecification*>((*spec).get());
    EXPECT_EQ(TestInterpreterState::MockRuleSpecification(1), *actualSpec);

    spec = state.getNextRuleSpec();
    EXPECT_NE(spec, std::nullopt);
    EXPECT_EQ(state.ruleSpecsRemaining(), 0);
    actualSpec = dynamic_cast<const TestInterpreterState::MockRuleSpecification*>((*spec).get());
    EXPECT_EQ(TestInterpreterState::MockRuleSpecification(0), *actualSpec);


    spec = state.getNextRuleSpec();
    EXPECT_EQ(spec, std::nullopt);
};