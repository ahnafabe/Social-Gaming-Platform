#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <regex>

#include "external/GameServer.h"

using networking::Connection;
using networking::Message;
using networking::Server;
using json = nlohmann::json;

class GameServerTest : public ::testing::Test
{
protected:
    std::unique_ptr<GameServer> gs;

    void SetUp() override
    {
        char validPath[] = "../src/external/src/external/web-socket-networking/webchat.html";
        char *ptrToPath = validPath;
        gs = std::make_unique<GameServer>(8007, ptrToPath);
    }

    void TearDown() override
    {
    }
};

TEST(GameServerTestManual, ConstructWithInvalidHtmlPath)
{
    char invalidPath[] = "index.html";
    char *ptrToPath = invalidPath;

    EXPECT_ANY_THROW({ GameServer gs(8001, ptrToPath); });
}

TEST(GameServerTestManual, ConstructWithValidHtmlPath)
{
    char validPath[] = "../src/external/src/external/web-socket-networking/webchat.html";
    char *ptrToPath = validPath;

    EXPECT_NO_THROW({ GameServer gs(8001, ptrToPath); });
}

TEST_F(GameServerTest, ClientConnection)
{

    Connection conn_1;
    conn_1.id = 123;

    EXPECT_NO_THROW(gs->onConnect(conn_1));

    gs->onDisconnect(conn_1);
}

TEST_F(GameServerTest, SizeForMultipleClientConnection)
{

    Connection conn_1;
    conn_1.id = 123;

    Connection conn_2;
    conn_2.id = 124;

    Connection conn_3;
    conn_3.id = 125;

    gs->onConnect(conn_1);
    gs->onConnect(conn_2);
    gs->onConnect(conn_3);

    // Size Comparison
    size_t expected = 3;
    size_t result = gs->getClients().size();
    EXPECT_EQ(expected, result);

    gs->onDisconnect(conn_1);
    gs->onDisconnect(conn_2);
    gs->onDisconnect(conn_3);
}

TEST_F(GameServerTest, MultipleClientConnection)
{

    Connection conn_1;
    conn_1.id = 123;

    Connection conn_2;
    conn_2.id = 124;

    Connection conn_3;
    conn_3.id = 125;

    gs->onConnect(conn_1);
    gs->onConnect(conn_2);
    gs->onConnect(conn_3);

    // Since clientID is unique, compare client ID
    EXPECT_EQ(gs->getClients().at(0).id, 123);
    EXPECT_EQ(gs->getClients().at(1).id, 124);
    EXPECT_EQ(gs->getClients().at(2).id, 125);

    gs->onDisconnect(conn_1);
    gs->onDisconnect(conn_2);
    gs->onDisconnect(conn_3);
}

TEST_F(GameServerTest, ClientDisconnection)
{

    Connection conn_1;
    conn_1.id = 123;

    Connection conn_2;
    conn_2.id = 124;

    Connection conn_3;
    conn_3.id = 125;

    gs->onConnect(conn_1);
    gs->onConnect(conn_2);
    gs->onConnect(conn_3);

    gs->onDisconnect(conn_2);

    // Size Comparison
    size_t expected = 2;
    size_t result = gs->getClients().size();
    EXPECT_EQ(expected, result);

    // Since clientID is unique, compare client ID
    EXPECT_EQ(gs->getClients().at(0).id, 123);
    EXPECT_EQ(gs->getClients().at(1).id, 125);

    gs->onDisconnect(conn_1);
    gs->onDisconnect(conn_3);
}

// Test the shutdown handling of the server
TEST_F(GameServerTest, HandleShutdown)
{

    Message shutdownMessage;
    shutdownMessage.text = "shutdown";

    // Call handleShutDown and verify the response
    MessageResult result = gs->handleShutDown(shutdownMessage);
    EXPECT_TRUE(result.shouldShutdown);
}

// Test to process invalid message formats
TEST_F(GameServerTest, HandleInvalidMessage)
{

    Message invalidMessage;
    invalidMessage.text = "INVALID!!";

    std::ostringstream os;
    MessageResult result = gs->handleMessage(invalidMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("Invalid Message!"));
    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, ProcessValidMessageEcho)
{

    Message validMessage;
    validMessage.text = R"({"action":"2","body":"Text To Echo", "request_id":"3"})";

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(validMessage, os);

    EXPECT_EQ(result.result, "{\"game_session_id\":\"gameIdXXXX\",\"message\":\"Text To Echo\",\"request_id\":\"3\",\"success\":true,\"type\":20}");
    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, SessionConnectionTest)
{

    std::unordered_map<std::string, std::vector<Connection>> sessions;
    sessions["session1"] = {{123}, {124}, {125}};

    std::vector<Connection> sessionConnections = gs->getSessionConnection(sessions, "session1");

    EXPECT_EQ(sessionConnections.size(), 3);
    EXPECT_EQ(sessionConnections[0].id, 123);
    EXPECT_EQ(sessionConnections[1].id, 124);
    EXPECT_EQ(sessionConnections[2].id, 125);
}

TEST_F(GameServerTest, CheckSessionCreation)
{

    Message validMessage;
    validMessage.text = R"({"action":"3","body":"any", "request_id":"1"})";

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(validMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("\"client_ids\":"));

    // Note that the session ID is randomly genearted
    EXPECT_THAT(result.result, ::testing::HasSubstr("],\"game_session_id\":\"1\",\"message\":\"Join Code: VP52M8\",\"request_id\":\"1\",\"success\":true,\"type\":20}"));
    std::regex sessionIdRegex("\"game_session_id\":\"[A-Za-z0-9]+\"");
    EXPECT_TRUE(std::regex_search(result.result, sessionIdRegex));

    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, CheckTwoSessionsCreationByID)
{

    // First Creation
    Message validMessage1;
    validMessage1.text = R"({"action":"3","body":"first", "request_id":"1"})";

    // Second Creation
    Message validMessage2;
    validMessage2.text = R"({"action":"3","body":"second", "request_id":"1"})";

    std::ostringstream os;

    MessageResult result1 = gs->processValidMessage(validMessage1, os);
    MessageResult result2 = gs->processValidMessage(validMessage2, os);

    // Since creation of each is tested already, skipped

    // The join codes (game_session_id) should be different
    auto extractSessionId = [](const std::string &result) -> std::string
    {
        std::regex sessionIdRegex("\"game_session_id\":\"([A-Za-z0-9]+)\"");
        std::smatch match;
        if (std::regex_search(result, match, sessionIdRegex) && match.size() > 1)
        {
            return match.str(1); // Return the matched session ID
        }
        return "";
    };

    std::string sessionId1 = extractSessionId(result1.result);
    std::string sessionId2 = extractSessionId(result2.result);

    EXPECT_NE(sessionId1, sessionId2);
    EXPECT_FALSE(result1.shouldShutdown);
    EXPECT_FALSE(result2.shouldShutdown);
}

TEST_F(GameServerTest, CheckTwoSessionsCreationByJoinCode)
{

    // First Creation
    Message validMessage1;
    validMessage1.text = R"({"action":"3","body":"first", "request_id":"1"})";

    // Second Creation
    Message validMessage2;
    validMessage2.text = R"({"action":"3","body":"second", "request_id":"1"})";

    std::ostringstream os;

    MessageResult result1 = gs->processValidMessage(validMessage1, os);
    MessageResult result2 = gs->processValidMessage(validMessage2, os);

    auto extractSessionJoinCode = [](const std::string &result) -> std::string
    {
        std::regex sessionJoinCodeRegex("\"message\":\"Join Code: ([A-Za-z0-9]+)\"");
        std::smatch match;
        if (std::regex_search(result, match, sessionJoinCodeRegex) && match.size() > 1)
        {
            return match.str(1); // Return the matched session ID
        }
        return "";
    };

    std::string sessionCode1 = extractSessionJoinCode(result1.result);
    std::string sessionCode2 = extractSessionJoinCode(result2.result);

    EXPECT_NE(sessionCode1, sessionCode2);
    EXPECT_FALSE(result1.shouldShutdown);
    EXPECT_FALSE(result2.shouldShutdown);
}

TEST_F(GameServerTest, CheckJoinSessionSuccess)
{

    Message sessionCreationMessage;
    sessionCreationMessage.text = R"({"action":"3","body":"first", "request_id":"1"})";

    std::ostringstream os;

    MessageResult result = gs->processValidMessage(sessionCreationMessage, os);

    auto extractSessionJoinCode = [](const std::string &result) -> std::string
    {
        std::regex sessionJoinCodeRegex("\"message\":\"Join Code: ([A-Za-z0-9]+)\"");
        std::smatch match;
        if (std::regex_search(result, match, sessionJoinCodeRegex) && match.size() > 1)
        {
            return match.str(1); // Return the matched session ID
        }
        return "";
    };

    std::string sessionCode = extractSessionJoinCode(result.result);

    std::ostringstream joinMessageStream;
    joinMessageStream << R"({"action":"0","body":")" << sessionCode << R"(", "request_id":"2"})";

    Message joinSessionMessage;
    joinSessionMessage.text = joinMessageStream.str();

    result = gs->processValidMessage(joinSessionMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("\"game_session_id\":\"1\""));
    EXPECT_THAT(result.result, ::testing::HasSubstr("[JOIN] Player joined the session"));
    EXPECT_THAT(result.result, ::testing::HasSubstr("\"request_id\":\"2\""));
    EXPECT_THAT(result.result, ::testing::HasSubstr("\"success\":true"));

    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, CheckJoinSessionFailure)
{

    Message joinSessionMessage;
    joinSessionMessage.text = R"({"action":"0","body":"wrong", "request_id":"2"})";
    std::ostringstream os;

    MessageResult result = gs->processValidMessage(joinSessionMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("\"game_session_id\":\"N/A\""));
    EXPECT_THAT(result.result, ::testing::HasSubstr("[JOIN] Session with specified join code not found"));
    EXPECT_THAT(result.result, ::testing::HasSubstr("\"success\":false"));

    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, HandleUnknownAction)
{
    Message unknownActionMessage;
    unknownActionMessage.text = R"({"action":"999","body":"Unknown Action", "request_id":"4"})";

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(unknownActionMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("Unknown action!"));
    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, HandleMissingActionField)
{
    Message missingActionMessage;
    missingActionMessage.text = R"({"body":"Missing Action", "request_id":"5"})";

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(missingActionMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("Invalid JSON format!"));
    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, HandleMalformedJSON)
{
    Message malformedJSONMessage;
    malformedJSONMessage.text = R"({"action":"2","body":"Malformed JSON", "request_id":"6")"; // Missing closing brace

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(malformedJSONMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("Invalid JSON format!"));
    EXPECT_FALSE(result.shouldShutdown);
}

TEST_F(GameServerTest, BroadcastMessageToAllClients)
{
    // Create and connect multiple clients
    Connection conn_1;
    conn_1.id = 101;

    Connection conn_2;
    conn_2.id = 102;

    Connection conn_3;
    conn_3.id = 103;

    gs->onConnect(conn_1);
    gs->onConnect(conn_2);
    gs->onConnect(conn_3);

    Message broadcastMessage;
    broadcastMessage.text = R"({"action":"2","body":"Broadcast Message", "request_id":"7"})";

    std::ostringstream os;
    MessageResult result = gs->processValidMessage(broadcastMessage, os);

    EXPECT_THAT(result.result, ::testing::HasSubstr("<101> Broadcast Message"));
    EXPECT_THAT(result.result, ::testing::HasSubstr("<102> Broadcast Message"));
    EXPECT_THAT(result.result, ::testing::HasSubstr("<103> Broadcast Message"));

    EXPECT_FALSE(result.shouldShutdown);

    gs->onDisconnect(conn_1);
    gs->onDisconnect(conn_2);
    gs->onDisconnect(conn_3);
}