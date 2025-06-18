#include "Request.h"
#include <nlohmann/json.hpp> // Library for parsing JSON
#include <iostream>

using json = nlohmann::json;


//Constructor for request
Request::Request(const std::string& message, Connection client)
    : client(client) {
    //Parse JSON using nlohmann library and initilize Request

    try { 
        // Parse string into JSON object
        std::cout << "Message: " << message << std::endl;
        json jsonRequest = json::parse(message);

        //Set fields from JSON object
        if(jsonRequest.contains("action")) {
            // Get and convert the stringed integer to MessageType
            int intAction = std::stoi(jsonRequest["action"].get<std::string>());
            action = static_cast<MessageType>(intAction);
        }
        
        if(jsonRequest.contains("body")) {
            std::string strBody = jsonRequest["body"].get<std::string>();
            body = strBody;
        }

        if(jsonRequest.contains("sessionID")) {
            std::string strSession = jsonRequest["sessionID"].get<std::string>();
            sessionId = strSession;
        }

        if(jsonRequest.contains("request_id")) {
            std::string strRequestId = jsonRequest["request_id"].get<std::string>();
            requestId = strRequestId;
        }

        if(jsonRequest.contains("target_var")) {
            targetVar = jsonRequest["target_var"].get<std::string>();
        }

        isValid = true;

    } catch (json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    }
}