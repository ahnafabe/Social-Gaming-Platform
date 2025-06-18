#include "GameProcess.h"
#include "tree_sitter/TSParser.h"

namespace logic
{
    Interpreter createInterpreter(std::string sourceCode, Session *session)
    {
        TSParser parser{sourceCode};

        auto ruleSpecs = parser.parseRuleSpecs();

        if (!ruleSpecs.has_value())
        {
            throw std::invalid_argument("Failed to parse rule specs");
        }

        Interpreter interpreter{ruleSpecs.value(), session};

        return interpreter;
    }

    GameProcess::GameProcess(Session *session, std::string sourceCode)
        : session(session), interpreter(createInterpreter(sourceCode, session))
    {
    }

    int GameProcess::getSessionId() const noexcept
    {
        return session->getId();
    }

    LastOutcome GameProcess::getLastInterpreterOutcome() const noexcept
    {
        return interpreter.getState().getLastOutcome();
    }

    [[nodiscard]] RuleExecutionOutcome 
    GameProcess::execute() noexcept
    {
        return interpreter.executeRules();
    }

    /*
     * Implementation of ProcessTraits<GameProcess>
     */
    
    ProcessTraits<GameProcess>::ProcessTraits(GameProcess process) : process(process) {};

    ProcessID ProcessTraits<GameProcess>::getId() const
    {
        return process.getSessionId();
    }

    bool ProcessTraits<GameProcess>::isWaitingForIO() const
    {
        return process.getLastInterpreterOutcome() == RuleExecutionOutcome::SUCCESS_WAITING_FOR_INPUT
            || process.getLastInterpreterOutcome() == RuleExecutionOutcome::SUCCESS_DELIVERING_OUTPUT;
    }

    bool ProcessTraits<GameProcess>::isDone() const
    {
        return process.getLastInterpreterOutcome() == RuleExecutionOutcome::NO_MORE_RULES_TO_EXECUTE
            || process.getLastInterpreterOutcome() == RuleExecutionOutcome::INTERNAL_FAILURE;
    }

    void ProcessTraits<GameProcess>::tick()
    {
        RuleExecutionOutcome result = process.execute();
    }
}
