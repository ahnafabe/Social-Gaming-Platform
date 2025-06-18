#pragma once

#include <unordered_map>
#include <optional>
#include <climits>
#include <expected>

#include "data/session/session.h"

class SessionManager
{
private:
  // Map from a session's id to session
  std::unordered_map<int, Session> sessions;

  int getNextId() const;
  std::string generateJoinCode() const;
  Session& newSession(const GameData &gameData);

public:
  static const int JOIN_CODE_LENGTH = 6;

  //Player creates session
  std::expected<Session*, std::string> createSession(uintptr_t playerID, const GameData &gameData);
  
  std::expected<void, std::string> destroySession(int sessionId);
  std::expected<Session*, std::string> getSession(int sessionId);
  std::expected<Session*, std::string> getSession(const std::string &joinCode);
  std::expected<Session*, std::string> findSessionByPlayer(uintptr_t playerID);
  bool isPlayerInAnySession(uintptr_t playerID);
  bool isSessionExists(const std::string &joinCode);
  bool isSessionExists(int sessionId);
  bool isPlayerInSession(const Session &session, uintptr_t playerID);

  // Add player to session using a join code
  std::expected<Session*, std::string> addPlayerToSession(const std::string& joinCode, const Connection& connection);
};
