#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include "MessageTypes.h"
#include "data/data.h"
#include "Server.h"
#include <cstdint>
#include <optional>

struct Request {
    //Possible parameters in the request
    MessageType action = MessageType::UNDEFINED;
    std::string body = "";
    std::string sessionId = "";
    std::string requestId = "";
    Connection client;
    std::optional<std::string> targetVar;
    bool isValid = false;

    //Constructor will parse the string message from server
    Request(const std::string& message, const Connection client);
};

#endif