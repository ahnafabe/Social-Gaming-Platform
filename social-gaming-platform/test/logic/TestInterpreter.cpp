#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "logic/interpreter/Interpreter.h"


namespace TestInterpreter {

    class MockRuleSpecification final : public logic::BaseRuleSpecification {
    public:
        const logic::RuleExecutionOutcome outcome;
        logic::RuleSpecs nestedRules;

        MockRuleSpecification(int nestedRulesCount, logic::RuleExecutionOutcome outcome)
            : logic::BaseRuleSpecification(logic::RuleType::ASSIGNMENT, nestedRulesCount),
              outcome(outcome) {}
        
        MockRuleSpecification(int nestedRulesCount, logic::RuleExecutionOutcome outcome, logic::RuleSpecs nestedRules)
            : logic::BaseRuleSpecification(logic::RuleType::ASSIGNMENT, nestedRulesCount),
              outcome(outcome), nestedRules(std::move(nestedRules)) {}

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

            auto* spec = dynamic_cast<TestInterpreter::MockRuleSpecification*>(ruleSpec.get());
            if (!spec) {
                return logic::ExecuteRuleResult{
                    logic::RuleExecutionOutcome::INTERNAL_FAILURE,
                    logic::RuleType::ASSIGNMENT
                };
            }

            // update the interpreterState
            for (auto nestedRuleSpec = spec->nestedRules.rbegin(); nestedRuleSpec != spec->nestedRules.rend(); nestedRuleSpec++) {
                interpreterState.addRuleSpec(std::move(*nestedRuleSpec));
            }

            return logic::ExecuteRuleResult{
                spec->outcome,
                logic::RuleType::ASSIGNMENT,
            };
        }
    }; // end of MockRule

}


/* HELPER FUNCTION */

logic::Interpreter interpreterSetup(logic::RuleSpecStack ruleSpecStack, Session& session) {
    logic::Interpreter interpreter (ruleSpecStack, &session);
    bool registered = interpreter.registerRule(logic::RuleType::ASSIGNMENT, std::make_shared<TestInterpreter::MockRule>());
    return std::move(interpreter);
}


TEST(InterpreterTests, NoRulesToExecute) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;
    logic::Interpreter interpreter = interpreterSetup(std::move(ruleSpecStack), session);

    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE);
};


TEST(InterpreterTests, RegisterDuplicateRule) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;
    logic::Interpreter interpreter = interpreterSetup(std::move(ruleSpecStack), session);

    bool registered = interpreter.registerRule(logic::RuleType::ASSIGNMENT, std::make_shared<TestInterpreter::MockRule>());
    EXPECT_FALSE(registered);
};


TEST(InterpreterTests, OneRuleNoNestingRules) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING));
    

    logic::Interpreter interpreter = interpreterSetup(std::move(ruleSpecStack), session);

    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE);
};


TEST(InterpreterTests, RulesWithIOWaiting) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING));
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT));
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT));
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING));

    logic::Interpreter interpreter = interpreterSetup(std::move(ruleSpecStack), session);

    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT);
    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT);
    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE);
};


TEST(InterpreterTests, NestedRules) {
    logic::RuleSpecStack ruleSpecStack;
    Session session;

    logic::RuleSpecs nestedRules;
    
    nestedRules.emplace_back(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT));
    nestedRules.emplace_back(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT));
    
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        0, logic::RuleExecutionOutcome::SUCCESS_WITH_NO_NESTED_RULES_REMAINING));
    ruleSpecStack.push(std::make_shared<TestInterpreter::MockRuleSpecification>(
        1, logic::RuleExecutionOutcome::SUCCESS_WITH_NESTED_RULES_REMAINING, std::move(nestedRules)));
    
    logic::Interpreter interpreter = interpreterSetup(std::move(ruleSpecStack), session);

    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT);
    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT);
    EXPECT_EQ(interpreter.executeRules(), logic::RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE);
};