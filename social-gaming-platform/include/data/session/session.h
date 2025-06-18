#pragma once

#include "data/session/player.h"

#include "data/data_node.h"
#include "data/configuration.h"
#include "data/game_state_object.h"
#include <iostream>

#include "Server.h"

using networking::Connection;

struct GameData
{
  Configuration configuration;
  GameStateObject constants;
  GameStateObject variables;
  GameStateObject perPlayerState;
  GameStateObject perAudienceState;
};

struct Session
{
private:
  int id;
  std::string joinCode;
  GameData gameData;
  std::list<Player> players;

public:
  Session() : id(-1), gameData(GameData()), joinCode(""), players(std::list<Player>()) {};
  Session(int id, GameData gameData, std::string joinCode) : id(id),
                                                             gameData(gameData),
                                                             joinCode(joinCode),
                                                             players(std::list<Player>()) {};

  int getId() const { return id; };
  std::string getJoinCode() const { return joinCode; };
  GameData& getGameData() { return gameData; };
  const std::list<Player> getPlayers() const { return players; };

  /**
   * Add Player (using player ID) into session
   * @param connection clientID
   * @todo name should be ... what?
   */
  void addPlayer(Connection client)
  {
    players.insert(players.end(), {client, client.id, "TempName"});
  };
};