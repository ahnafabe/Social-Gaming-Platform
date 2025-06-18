/**
 * GameServer
 *
 * Reference: https://github.com/nsumner/web-socket-networking
 */
#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <deque>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "MessageTypes.h"
#include "RequestHandler.h"
#include "data/data.h"
#include "data/session/manager.h"

#include "logic/scheduler/Scheduler.h"
#include "logic/GameProcess.h"

using networking::Connection;
using networking::Message;
using networking::Server;
using json = nlohmann::json;

/**
 * @brief Struct to hold results after processing the message
 */
struct MessageResult
{
    std::string result;                     // Response to send to client
    bool shouldShutdown;                    // Flag which indicates whether server to shut down
    std::vector<uintptr_t> sendToClientIDs; // specific clients to send to
};

class GameServer
{
public:
    /**
     * @brief Constructor
     *
     * @param port port number
     * @param htmlResponseFile Path to the HTML file
     */
    GameServer(unsigned short, char *&);

    /**
     * @brief Destructor
     */
    ~GameServer() noexcept = default;

    /**
     * @brief Start the game server and runs the event loop
     */
    void start();

    /**
     * @brief Clients
     */
    const std::vector<Connection> &getClients() const { return clients; };

    /**
     * @brief Connecting Client
     */
    void onConnect(Connection c);

    /**
     * @brief Disconnecting Client
     */
    void onDisconnect(Connection c);

    /**
     * @brief Handling shutdown
     *
     * @param msg Message from clients
     */
    MessageResult handleShutDown(const Message &);

    /**
     * @brief Invalid messages when msg is not JSON format.
     *
     * @param msg Message from client
     * @param result Invalid Message result to be shown
     */
    MessageResult handleMessage(const Message &, std::ostringstream &);

    /**
     * @brief Process valid client message (JSON formatted).
     *
     * @param msg message from client
     * @param result message response
     */
    MessageResult processValidMessage(const Message &, std::ostringstream &);

    /**
     * @brief Processes outgoing messages from the outgoing queue.
     */
    MessageResult processOutgoingMessages();

    /**
     * @brief Extracts client IDs from the response.
     */
    std::vector<uintptr_t> extractClientIdsFromResponse(const Response &);

    /**
     * @brief Process the message
     *
     * @param s Server
     * @param incoming Incoming message
     */
    MessageResult processMessages(const std::deque<Message> &);

    /**
     * @brief Message building and sending
     *
     * @param players The map of session codes to player connections.
     * @param sessionCode The code identifying the session.
     */
    std::deque<Message> buildOutgoing(const std::string &, const std::expected<Session *, std::string> &, const std::vector<uintptr_t> &);

    /**
     * @brief Get the session Players - for later usage
     */
    std::vector<Connection> getSessionConnection(const std::unordered_map<std::string, std::vector<Connection>> &, const std::string);

    /**
     * @brief Loads the HTTP response message from the specified HTML file.
     *
     * @param htmlLocation path to the html
     */
    std::string getHTTPMessage(const char *htmlLocation);

    /**
     * @brief Running the server
     */
    void run();

    /**
     * @brief Stop the server
     * For later, resource cleaner
     * Now, it is just printing
     */
    void stop();

private:
    Server server;
    SessionManager sessionManager;                                  // Manages sessions
    // RequestHandler requestHandler = RequestHandler(sessionManager); // Handles incoming request
    GameManager gameManager = GameManager("../../games");                        
    logic::Scheduler<logic::GameProcess> scheduler = logic::Scheduler<logic::GameProcess>();
    RequestHandler requestHandler = RequestHandler(sessionManager, gameManager, scheduler);
    
    std::vector<Connection> clients;    // Connected clients
    std::queue<Request> incomingQueue;  // Queue for incoming
    std::queue<Response> outgoingQueue; // Queue for outgoing

    /**
     * @brief Handle incoming and outgoing messages
     */
    bool handleServerUpdates();


    /**
     * @brief Run game processes
     */
    bool handleGameUpdates();
};
