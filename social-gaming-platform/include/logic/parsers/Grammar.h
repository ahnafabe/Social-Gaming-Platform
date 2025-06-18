#pragma once

#include <optional>
#include <string_view>


namespace logic {
    
    extern const std::string_view COMPARE_EQ; 
    extern const std::string_view COMPARE_NOT_EQ; 
    extern const std::string_view COMPARE_LESS_THAN; 
    extern const std::string_view COMPARE_LESS_THAN_EQ; 
    extern const std::string_view COMPARE_GREATER_THAN; 
    extern const std::string_view COMPARE_GREATER_THAN_EQ; 

    extern const std::string_view LOGICAL_NOT; 
    extern const std::string_view LOGICAL_OR; 
    extern const std::string_view LOGICAL_AND; 

    extern const std::string_view PLUS_OP;
    extern const std::string_view MINUS_OP;
    extern const std::string_view MULTIPLY_OP;
    extern const std::string_view DIVIDE_OP;
    extern const std::string_view MODULUS_OP;


    /**
     * Types for the SocialGaming language.
     */
    enum class SocialGamingType {
        BOOLEAN,
        IDENTIFIER,
        NUMBER,
        NUMBER_RANGE,
        STRING,
        UNKNOWN
    };
    
    
    /**
     * Types of rules for the interpreter.
     */
    enum class RuleType {
        // IO rules
        INPUT_CHOICE, // TODO: LOGIC-16
        INPUT_RANGE, // TODO: LOGIC-18
        INPUT_TEXT, // TODO: LOGIC-17
        INPUT_VOTE, // TODO: LOGIC-19
        OUTPUT_MESSAGE, // TODO: LOGIC-14
        OUTPUT_SCORE, // TODO: LOGIC-15

        // list operations
        LIST_CONTAINS, // TODO
        LIST_DEAL, // TODO
        LIST_DISCARD, // TODO
        LIST_ELEMENTS, // TODO
        LIST_EXTEND, // TODO
        LIST_REVERSE, // TODO
        LIST_SHUFFLE, // TODO
        LIST_SIZE, // TODO
        LIST_SORT, // TODO

        // control structures
        LOOP_FOR, // TODO: LOGIC-20
        LOOP_IN_PARALLEL, // TODO: LOGIC-23
        LOOP_PARALLEL_FOR, // TODO: LOGIC-22
        LOOP_WHILE, // TODO: LOGIC-21
        MATCH, // TODO: LOGIC-24

        // misc rules
        ASSIGNMENT, // TODO
        TIMER, // TODO
        UPFROM, // TODO
        UNKNOWN
    }; // end of RuleType


    /**
     * Convert a string to a SocialGamingType.
     * If a type can not be determined from the string
     * 
     * @param dataType String representation of a SocialGaming data type.
     */
    [[nodiscard]] std::optional<SocialGamingType> 
    toSocialGamingType(std::string_view dataType) noexcept;


}
