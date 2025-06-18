#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <optional>
#include <variant>
#include "MessageTypes.h"


//All responses share these attributes
struct CommonResponse {
    const std::string gameSessionId;
    const std::string message; //prompt or general message from server
    const MessageType type;
    const std::vector<uintptr_t> clientIds; //optional -- specify clients to respond to

    //Optional to specify direct response to client request
    //Implementation can be used by client to resend unanswered requests
    const std::optional<bool> success;
    const std::optional<std::string> requestId;

    //Constructor for the response
    CommonResponse(
        std::string gameSessionId,
        std::string message,
        MessageType type,
        const std::vector<uintptr_t>& clientIds = {},
        std::optional<bool> success = std::nullopt,
        std::optional<std::string> requestId = std::nullopt
    ) : gameSessionId(gameSessionId),
        message(message),
        type(type),
        clientIds(clientIds),
        success(success),
        requestId(requestId) {}
};


//base response -- no extra attributes
struct MessageResponse {
    const CommonResponse common;

    // Constructor
    MessageResponse(const CommonResponse& commonResponse)
        : common(commonResponse) {}
};


//response requesting inputs
struct InputResponse {
    const CommonResponse common;
    const std::string_view targetVar;
    const std::optional<int> timeout;

    // Constructor
    InputResponse(
        const CommonResponse& commonResponse,
        std::string_view targetVar, //variable to store input into

        //timeout for input (optional)
        std::optional<int> timeout = std::nullopt
    ) : common(commonResponse),
        targetVar(targetVar),
        timeout(timeout) {}
};

//defining response variants
using Response = std::variant<MessageResponse, InputResponse>;


//return serialized JSON of response
std::string serializeResponse(const Response& response);

#endif