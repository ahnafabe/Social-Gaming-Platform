#include "Response.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


std::string serializeResponse(const Response& response) {

    //std::visit uses correct variant of response
    return std::visit([](const auto& res) -> std::string {
        json jsonResponse;

        //First assign common attributes
        const auto& common = res.common;
        jsonResponse["game_session_id"] = common.gameSessionId;
        jsonResponse["message"] = common.message;
        jsonResponse["type"] = static_cast<int>(common.type);

        //Optional attributes
        if (!common.clientIds.empty()) {
            jsonResponse["client_ids"] = common.clientIds;
        }
        if (common.success.has_value()) {
            jsonResponse["success"] = common.success.value();
        }
        if (common.requestId.has_value()) {
            jsonResponse["request_id"] = common.requestId.value();
        }

        //Now check to see type of variant at compile time to assign more attributes
        using T = std::decay_t<decltype(res)>;
        if constexpr (std::is_same_v<T, InputResponse>){
            //Extra attributes from InputResponse
            jsonResponse["target_var"] = res.targetVar;
            if (res.timeout.has_value()) {
                jsonResponse["timeout"] = res.timeout.value();
            }
        } else if constexpr (std::is_same_v<T, MessageResponse>){
            //no extra attributes from MessageResponse
        }
        return jsonResponse.dump();

    }, response);

}