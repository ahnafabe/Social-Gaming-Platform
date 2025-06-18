#pragma once

#include <expected>
#include <optional>
#include <stack>

#include "data/session/session.h"
#include "RuleSupport.h"
#include "parsers/rule_specifications/BaseRuleSpecification.h"

namespace logic
{
    using LastOutcome = std::expected<RuleExecutionOutcome, nullptr_t>;

    class InterpreterState
    {
    public:
        InterpreterState(RuleSpecStack ruleSpecStack, Session *session);

        /**
         * Pop and return the next rule off from the ruleSpecStack.
         *
         */
        [[nodiscard]] std::optional<std::shared_ptr<BaseRuleSpecification>>
        getNextRuleSpec() noexcept;

        /**
         * Add a rule the the ruleSpeckStack.
         *
         * @param ruleSpec Rule specification to be added the the ruleSpecStack.
         */
        void addRuleSpec(std::shared_ptr<BaseRuleSpecification> ruleSpec) noexcept;

        /**
         * Return the number of rules remaining in ruleSpecStack.
         */
        [[nodiscard]] int ruleSpecsRemaining() const noexcept;

        /**
         * Return a reference to the data contents.
         */
        [[nodiscard]] Session *getSession() const noexcept;

        /**
         * Return the outcome of the most recent rule execution.
         * If execution has no started, return a void pointer (nullptr).
         */
        [[nodiscard]] LastOutcome getLastOutcome() const noexcept;

        /**
         * Update the interpreter state with the most recent
         * rule execution outcome.
         */
        void setLastOutcome(RuleExecutionOutcome outcome) noexcept;

    private:
        RuleSpecStack ruleSpecStack;
        Session *session;
        /**
         * NULL when no rules have been executed yet.
         */
        LastOutcome lastOutcome = std::unexpected(nullptr);
        // TODO: add IO messaging queue

    }; // end of InterpreterState
}
