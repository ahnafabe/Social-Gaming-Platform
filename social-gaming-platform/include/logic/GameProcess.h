#pragma once

#include "IParser.h"
#include "ProcessTraits.h"
#include "Interpreter.h"

#include "data/session/session.h"


namespace logic
{

    class GameProcess
    {
    public:
        GameProcess(Session *session, std::string sourceCode);

        [[nodiscard]] int getSessionId() const noexcept;

        // Interpreter methods
        LastOutcome getLastInterpreterOutcome() const noexcept;
        [[nodiscard]] RuleExecutionOutcome execute() noexcept;

    private:
        Session *session;
        Interpreter interpreter;
    };


    template <>
    class ProcessTraits<GameProcess>
    {
        using ProcessKind = GameProcess;

    private:
        ProcessKind process;

    public:
        ProcessTraits(ProcessKind process);

        ProcessID getId() const;
        void tick();
        bool isWaitingForIO() const;
        bool isDone() const;
    };
}