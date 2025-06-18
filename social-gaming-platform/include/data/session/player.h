#pragma once
#include <string>

#include "Server.h"

using networking::Connection;

class Player
{
private:
  uintptr_t id;
  std::string name;

  /**
   * Client Connection
   */
  Connection clientConnection;

public:
  Player(uintptr_t id, std::string name) : id(id), name(name) {};
  Player(Connection c, uintptr_t id, std::string name) : clientConnection{c}, id{id}, name{name} {};
  uintptr_t getId() const { return id; };
  std::string getName() const { return name; };

  /**
   * Get the connection of player
   */
  Connection getConnection() const {return clientConnection; };
};
