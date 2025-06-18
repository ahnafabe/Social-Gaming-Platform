/**
 * GameServer.cpp
 */
#include "GameServer.h"

GameServer::GameServer(unsigned short port, char *&htmlResponseFile)
    : server(port, getHTTPMessage(htmlResponseFile), [this](Connection c)
             { this->onConnect(c); }, // Lambda to call onConnect
             [this](Connection c)
             { this->onDisconnect(c); }) // Lambda to call onDisconnect
{
}

void GameServer::onConnect(Connection c)
{
    // Alert New connection
    std::cout << "<" << c.id << "> New Client Connected\n";

    // Store c (= Client) into clients list
    clients.push_back(c);
}

void GameServer::onDisconnect(Connection c)
{
    // Alert disconnection
    std::cout << "<" << c.id << "> Client Disconnected\n";

    // Erase the disconnected c (= Client)
    auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
    clients.erase(eraseBegin, std::end(clients));
}

MessageResult
GameServer::handleShutDown(const Message &msg)
{
    std::string msgText = msg.text;
    std::transform(msgText.begin(), msgText.end(), msgText.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    if (msgText == "shutdown")
    {
        return MessageResult{"", true, {}};
    }
    return MessageResult{"", false, {}};
}

MessageResult
GameServer::handleMessage(const Message &msg, std::ostringstream &result)
{
    // To server
    std::cout << "<" << msg.connection.id << "> sends Invalid Message \"" << msg.text << "\"\n";

    // To client
    result << "<" << msg.connection.id << "> " << "Invalid Message!" << std::endl;

    return MessageResult{result.str(), false, {}};
}

MessageResult
GameServer::processValidMessage(const Message &msg, std::ostringstream &result)
{
    // Create Request object from incoming message
    Request request(msg.text, msg.connection);

    // If the request is invalid (not a JSON message), return error response
    if (!request.isValid)
    {
        result << "<" << msg.connection.id << "> Invalid Message!" << std::endl;
        return MessageResult{result.str(), false, {}};
    }

    // Add the request to the incoming queue
    incomingQueue.push(request);

    // Process the request
    while (!incomingQueue.empty())
    {
        Request request = incomingQueue.front();
        incomingQueue.pop();

        // Generate response for the request
        Response response = requestHandler.handleRequest(request);

        // Add response to outgoing queue
        outgoingQueue.push(response);
    }

    // Process and send outgoing messages
    return processOutgoingMessages();
}

MessageResult
GameServer::processOutgoingMessages()
{
    // Handle responses from the outgoing queue
    while (!outgoingQueue.empty())
    {
        Response response = outgoingQueue.front();
        outgoingQueue.pop();

        // Get list of client IDs to send the response to
        std::vector<uintptr_t> responseClientIDs = extractClientIdsFromResponse(response);

        // Return the serialized response to be sent
        return MessageResult{serializeResponse(response), false, responseClientIDs};
    }

    // Return empty if no outgoing responses to send
    return MessageResult{"", false, {}};
}

std::vector<uintptr_t>
GameServer::extractClientIdsFromResponse(const Response &response)
{
    std::vector<uintptr_t> clientIDs;

    std::visit([&clientIDs](const auto &resp)
               {
        const auto& common = resp.common;
        for (const auto& id : common.clientIds)
        {
            clientIDs.push_back(id);
        } }, response);

    return clientIDs;
}

MessageResult
GameServer::processMessages(const std::deque<Message> &incoming)
{
    std::ostringstream result;

    for (const auto &msg : incoming)
    {
        MessageResult shutDownResult = handleShutDown(msg);
        if (shutDownResult.shouldShutdown)
        {
            return shutDownResult;
        }

        try
        {
            return processValidMessage(msg, result);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
            return MessageResult{"Error processing message", false, {}};
        }
    }

    // Return empty response
    return MessageResult{result.str(), false, {}};
}

std::deque<Message>
GameServer::buildOutgoing(const std::string &log,
                          const std::expected<Session *, std::string> &sessionResult,
                          const std::vector<uintptr_t> &clientIDs)
{
    // Messages to send
    std::deque<Message> outgoing;

    try
    {
        // If player is in a session
        if (sessionResult.has_value())
        {
            Session *session = sessionResult.value();
            const auto &players = session->getPlayers();

            // If clientIDs was provided in response, only send to them
            if (!clientIDs.empty())
            {
                for (const auto &client : players)
                {
                    // If client in session is found in provided list, send to them
                    if (std::find(clientIDs.begin(), clientIDs.end(), client.getId()) != clientIDs.end())
                    {
                        outgoing.push_back({client.getConnection(), log});
                    }
                }
            }
            else
            {
                // Otherwise send to all in session - Iterate over each player in the session and send them the message
                for (const auto &client : players)
                {
                    outgoing.push_back({client.getConnection(), log});
                }
            }

            // Player is not in a session
        }
        else
        {
            if (!clientIDs.empty())
            {
                for (const auto &clientID : clientIDs)
                {
                    // Assuming clientID is the connection ID for the user
                    outgoing.push_back(Message{clientID, log});
                }
            }
            else
            {
                // If no clientIDs are provided, log an error
                std::cerr << "Error: Session not found and no client IDs provided to send the message." << std::endl;
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "No players found in Session" << std::endl;
    }

    return outgoing;
}

/**
 * Get the session Players
 */
std::vector<Connection>
GameServer::getSessionConnection(const std::unordered_map<std::string,
                                                          std::vector<Connection>> &players,
                                 const std::string sessionCode)
{
    // Look for Connections in the session
    auto it = players.find(sessionCode);

    // if found
    if (it != players.end())
    {
        return it->second;
    }
    else
    {
        // if not found, return empty
        return {};
    }
}

std::string
GameServer::getHTTPMessage(const char *htmlLocation)
{

    // if exists
    if (access(htmlLocation, R_OK) != -1)
    {
        std::ifstream infile{htmlLocation};
        return std::string{std::istreambuf_iterator<char>(infile),
                           std::istreambuf_iterator<char>()};
    }
    // Send error message if not exist
    std::cerr << "Unable to open HTML index file:\n"
              << htmlLocation << "\n";

    throw std::runtime_error(std::string("Unable to open HTML index file: ") + htmlLocation);
}

void GameServer::start()
{
    std::cout << "Game server starting..." << std::endl;
    run();
}

void GameServer::stop()
{
    std::cout << "\n\n******************************************\n";
    std::cout << "******** Shutting Down the Server ********\n";
    std::cout << "******************************************\n";
}

void GameServer::run()
{
    while (1)
    {
        try {
            bool serverUpdateSucceeded = handleServerUpdates();

            if (!serverUpdateSucceeded)
            {
                break;
            }

            bool gameUpdateSucceeded = handleGameUpdates();

            if (!gameUpdateSucceeded)
            {
                break;
            }

        } catch (const std::exception &e) {
            std::cerr << "Exception: " << typeid(e).name() << " - " << e.what() << std::endl;
        }
    }

}

bool GameServer::handleServerUpdates()
{
    try
    {
        // From Server.h
        server.update();
    }
    catch (std::exception &e) // Exception from server
    {
        std::cerr << "ERROR: Exception from Server update:\n"
                  << " " << e.what() << "\n\n";
        return false;
    }

    // Incoming from Server
    const auto incoming = server.receive();

    // Process Message from server
    const auto [log, shouldQuit, sendToClientIDs] = processMessages(incoming);

    // Find the session ID from the last processed message
    if (!incoming.empty())
    {
        const auto &lastMessage = incoming.back();
        uintptr_t playerID = lastMessage.connection.id;

        try
        {
            // Find if player has session
            auto sessionResult = sessionManager.findSessionByPlayer(playerID);

            // Build outgoing messages (passes sessionResult directly)
            const auto outgoing = buildOutgoing(log, sessionResult, sendToClientIDs);

            // Send the outgoing messages
            server.send(outgoing);
        }
        catch (const std::runtime_error &e)
        {
            if (!shouldQuit)
            {
                std::cout << "<" << playerID << "> Error: " << e.what() << std::endl;
            }
            else
            {
                std::cout << "<" << playerID << "> Error: " << e.what() << std::endl;
            }
        }

        if (shouldQuit)
        {
            stop();
        }

        // Every 1 second, it updates
        sleep(1);
    }

    return true;
}

bool GameServer::handleGameUpdates()
{
    try {
        scheduler.executeInParallel();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << typeid(e).name() << " - " << e.what() << std::endl;
        return false;
    }
    return true;
}