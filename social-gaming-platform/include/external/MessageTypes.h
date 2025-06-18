// MessageType.h
#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum class MessageType {
    // Actions initiated by the client
    JOIN = 0,
    END = 1,
    ECHO = 2,
    NEW_GAME = 3,

    // Responses that require client input
    INPUT_TEXT = 10,
    INPUT_CHOICE = 11,
    INPUT_RANGE = 12,
    INPUT_VOTE = 13,

    // Other response types
    MESSAGE = 20,
    SCORE = 21,

    // Undefined action
    UNDEFINED = -1,
};

#endif // MESSAGETYPE_H
