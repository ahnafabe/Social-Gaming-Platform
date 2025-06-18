#pragma once

#include "data_node.h"

class Configuration
{
private:
  DataNode config;

public:
  // Constructors
  Configuration();
  Configuration(const DataNode &config);

  // Getter for the DataNode
  const DataNode &getDataNode() const { return config; }

  // Other functions
  std::string_view getName() const;
  Range getPlayerRange() const;
  bool isAudienceEnabled() const;
};