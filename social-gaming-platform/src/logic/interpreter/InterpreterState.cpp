#include "InterpreterState.h"

namespace logic
{
    InterpreterState::InterpreterState(RuleSpecStack ruleSpecStack, Session *session)
        : ruleSpecStack(std::move(ruleSpecStack)), session(session) {}

    [[nodiscard]] std::optional<std::shared_ptr<BaseRuleSpecification>>
    InterpreterState::getNextRuleSpec() noexcept
    {
        if (ruleSpecStack.empty())
        {
            return std::nullopt;
        }

        auto spec = std::move(ruleSpecStack.top());
        ruleSpecStack.pop();
        return std::move(spec);
    } // end of getRule()

    void InterpreterState::addRuleSpec(std::shared_ptr<BaseRuleSpecification> ruleSpec) noexcept
    {
        ruleSpecStack.push(std::move(ruleSpec));
    } // end of addRule()

    [[nodiscard]] int
    InterpreterState::ruleSpecsRemaining() const noexcept
    {
        return ruleSpecStack.size();
    } // end of rulesRemaining()

    [[nodiscard]] Session *
    InterpreterState::getSession() const noexcept
    {
        return session;
    } // end of getSession()

    [[nodiscard]] LastOutcome
    InterpreterState::getLastOutcome() const noexcept
    {
        return lastOutcome;
    } // end of getLastOutcome()


    void InterpreterState::setLastOutcome(RuleExecutionOutcome outcome) noexcept
    {
        lastOutcome = outcome;
    } // end of setLastOutcome()
}