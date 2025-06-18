#include "data/configuration.h"

// Configuration Constructor defintions

Configuration::Configuration() : config(create_map_node())
{
}

Configuration::Configuration(const DataNode& config) : config(config)
{
}

// Configuration function definitions
std::string_view Configuration::getName() const
{
  const DataNode& node = config.getMapValue("name");

  if (!node.isString()) {
        throw std::runtime_error("DataNode does not contain a string for the key 'name'");
    }

  return node.getString();
}

Range Configuration::getPlayerRange() const
{
  const DataNode& node = config.getMapValue("players");

  if (!node.isRange()) {
        throw std::runtime_error("DataNode does not contain a Range for the key 'players'");
    }
    
  return node.getRange();
}

bool Configuration::isAudienceEnabled() const
{
  const DataNode& node = config.getMapValue("audience");

  if (!node.isBool()) {
        throw std::runtime_error("DataNode does not contain a bool for the key 'audience'");
    }
    
  return node.getBool();
}