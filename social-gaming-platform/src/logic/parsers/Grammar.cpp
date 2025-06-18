#include "Grammar.h"

namespace logic 
{
    const std::string_view COMPARE_EQ = "="; 
    const std::string_view COMPARE_NOT_EQ = "!="; 
    const std::string_view COMPARE_LESS_THAN = "<"; 
    const std::string_view COMPARE_LESS_THAN_EQ = "<="; 
    const std::string_view COMPARE_GREATER_THAN = ">"; 
    const std::string_view COMPARE_GREATER_THAN_EQ = ">="; 
    
    const std::string_view LOGICAL_NOT = "!";
    const std::string_view LOGICAL_OR = "||";
    const std::string_view LOGICAL_AND = "&&";
    
    const std::string_view PLUS_OP = "+";
    const std::string_view MINUS_OP = "-";
    const std::string_view MULTIPLY_OP = "*";
    const std::string_view DIVIDE_OP = "/";
    const std::string_view MODULUS_OP = "%";


    [[nodiscard]] std::optional<SocialGamingType> 
    toSocialGamingType(std::string_view dataType) noexcept {
        if (dataType == "boolean") return SocialGamingType::BOOLEAN;
        else if (dataType == "identifier") return SocialGamingType::IDENTIFIER;
        else if (dataType == "number") return SocialGamingType::NUMBER;
        else if (dataType == "quoted_string") return SocialGamingType::STRING;
        return std::nullopt;        
    } // end of toSocialGamingType()

}