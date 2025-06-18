#pragma once

#include "Request.h"
#include "Response.h"
#include "logic/scheduler/Scheduler.h"
#include "logic/GameProcess.h"

#include <algorithm>

class RequestHandler
{
public:
    // Request handler to be constructed with reference to sessionManager
    RequestHandler(SessionManager &sessionManager, GameManager &gameManager, logic::Scheduler<logic::GameProcess> &scheduler);

    // Receives Request object to handle and direct to specific endpoint
    // Returns response
    Response handleRequest(Request &request);

private:
    // Handlers specific to each endpoint

    Response handleJoin(Request &request);

    Response handleEnd(const Request &request) const;
    Response handleEcho(const Request &request) const;

    Response handleNewGame(Request &request);

    Response handleBadRequest(const Request &request) const;
    Response handleInputText(const Request &request) const;

    // Helper function to create error responses
    Response createErrorResponse(const Request &request, const std::string &errorMessage) const;

    logic::Scheduler<logic::GameProcess> &scheduler;
    SessionManager &sessionManager;
    GameManager &gameManager;
};
