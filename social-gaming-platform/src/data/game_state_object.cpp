#include "data/game_state_object.h"

// Variables Constructor definitions

GameStateObject::GameStateObject(const DataNode& variables) : variables(variables)
{
}

GameStateObject::GameStateObject() : variables(create_map_node())
{
}

// Variables function to get a variable by name
const DataNode& GameStateObject::getObjectByName(std::string_view key) const
{
    return variables.getMapValue(key);
}


// Variables function to set or update a variable by name
void GameStateObject::setObject(std::string key, const DataNode& value)
{
    variables.setMapValue(key, value);
}

// Variables function to remove a variable by name
void GameStateObject::removeObject(std::string_view key)
{
    variables.removeMapValue(key);
}