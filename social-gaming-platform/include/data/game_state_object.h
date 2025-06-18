#pragma once

#include "data_node.h"

class GameStateObject {
private:
    DataNode variables;

public:
    GameStateObject();
    GameStateObject(const DataNode& variables);

    const DataNode& getObjectByName(std::string_view key) const;
    void setObject(std::string key, const DataNode& value);

    void removeObject(std::string_view key);
};

