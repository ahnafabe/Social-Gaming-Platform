#include "RequestHandler.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Constructor implementation
RequestHandler::RequestHandler(SessionManager &sessionManager, GameManager &gameManager, logic::Scheduler<logic::GameProcess> &scheduler)
    : sessionManager(sessionManager), gameManager(gameManager), scheduler(scheduler) {};

Response RequestHandler::handleRequest(Request &request)
{
    // Logic for directing specific requests
    // May require more complex logic as endpoints change
    if (request.requestId.empty())
    {
        return handleBadRequest(request);
    }
    else
    {
        switch (request.action)
        {
        case (MessageType::JOIN):
            return handleJoin(request);
        case (MessageType::END):
            return handleEnd(request);
        case (MessageType::ECHO):
            return handleEcho(request);
        case (MessageType::NEW_GAME):
            return handleNewGame(request);
        case MessageType::INPUT_TEXT:
            return handleInputText(request);
        // add more when needed...
        default:
            return handleBadRequest(request);
        }
    }
}

// Example with InputResponse Type
/**
 * @brief Entering existed session via join code
 * @param request Request
 * @return response Message whether Session joined completed or denied
 * @todo Need to handle name of player
 */
Response RequestHandler::handleJoin(Request &request)
{
    std::cout << "\nJoin------" << std::endl; // debug

    const std::string &joinCode = request.body;

    // Attempt to add player to session
    auto addPlayerResult = sessionManager.addPlayerToSession(joinCode, request.client);

    if (addPlayerResult.has_value())
    {
        Session *session = addPlayerResult.value();

        // Success
        const int sessionId = session->getId();
        // Return success response
        CommonResponse commonRes(
            std::to_string(sessionId),
            "[JOIN] Player joined the session",
            MessageType::MESSAGE,
            {request.client.id},
            true,
            request.requestId);
        MessageResponse response(commonRes);
        return response;
    }
    else
    {
        // Failed through unexpected, create error response with the error message
        std::cout << "\tRH - Error: " << addPlayerResult.error() << std::endl; // debug
        return createErrorResponse(request, "[JOIN] " + addPlayerResult.error());
    }
}

/**
 * @todo stopping the game
 */
Response RequestHandler::handleEnd(const Request &request) const
{
    // Placeholder for an example of a potentally async call to LOGIC
    CommonResponse commonRes("gameIdXXXX", "[Game Ended]", MessageType::MESSAGE, {});
    MessageResponse response(commonRes);
    return response;
}

Response RequestHandler::handleEcho(const Request &request) const
{
    // Uncomment line below and replace {} to send only to client_id of sender (private response)
    // std::vector<uintptr_t> clientIds = {request.client.id};

    // Direct response from server
    CommonResponse commonRes("gameIdXXXX", request.body,
                             MessageType::MESSAGE, {}, true, request.requestId);
    MessageResponse response(commonRes);
    return response;
}

/**
 * @brief Handle Creation of new game
 * @param request Request
 * @return response Message whether New Session created or not
 */
Response RequestHandler::handleNewGame(Request &request)
{
    std::cout << "\nNEW GAME------\n";

    // Create new session
    auto sessionResult = sessionManager.createSession(request.client.id, GameData());

    if (sessionResult.has_value())
    {
        Session *newSession = sessionResult.value();

        // Get Game file specified
        auto sourceCode = gameManager.readGameFile(request.body);
        if (!sourceCode.has_value())
        {
            std::cout << "\tGame file specified not found" << std::endl; // debug
            return createErrorResponse(request, "[NEW GAME] Game file not found with specified name: " + request.body);
        }

        // If game type found
        logic::GameProcess newProcess(newSession, sourceCode.value().getContents());

        scheduler.addProcess(logic::ProcessTraits(newProcess));

        // Add player to session
        // @todo : Should be added as host of session instead of regular player
        newSession->addPlayer(request.client);

        std::cout << "\tNew session created with ID: " << newSession->getId() << std::endl; // debug
        std::cout << "\tPlayer added to new session" << std::endl;                          // debug
        // Return success response with join code
        std::string message = "Join Code: " + newSession->getJoinCode();
        CommonResponse commonRes(
            std::to_string(newSession->getId()),
            message,
            MessageType::MESSAGE,
            {request.client.id},
            true,
            request.requestId);
        MessageResponse response(commonRes);
        return response;
    }
    else
    {
        std::cout << "\tRH - Error: " << sessionResult.error() << std::endl; // debug
        return createErrorResponse(request, "[NEW GAME] " + sessionResult.error());
    }
}

Response RequestHandler::handleInputText(const Request &request) const
{
    // Process the input text from the client
    if (request.targetVar.has_value())
    {
        std::string_view inputVariable = request.targetVar.value();
        std::string_view inputValue = request.body;

        // Placeholder for logic with logic with inputVariable and inputValue

        // Prepare a response (if needed)
        CommonResponse commonRes(request.sessionId, "Input received",
                                 MessageType::MESSAGE, {request.client.id}, true, request.requestId);
        MessageResponse response(commonRes);
        return response;
    }
    else
    {
        return handleBadRequest(request);
    }
}

Response RequestHandler::handleBadRequest(const Request &request) const
{
    // Placeholder for error request
    CommonResponse commonRes("gameIdXXXX", "400 Bad Request",
                             MessageType::MESSAGE, {}, false, request.requestId);
    MessageResponse response(commonRes);
    return response;
}

Response RequestHandler::createErrorResponse(const Request &request, const std::string &errorMessage) const
{
    CommonResponse commonRes(
        request.sessionId.empty() ? "N/A" : request.sessionId,
        errorMessage,
        MessageType::MESSAGE,
        {request.client.id},
        false,
        request.requestId);
    MessageResponse response(commonRes);
    return response;
}
