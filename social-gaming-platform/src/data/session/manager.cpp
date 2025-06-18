#include "data/session/manager.h"
#include "data/session/session.h"
#include "data/session/helpers.h"

std::string SessionManager::generateJoinCode() const
{
  return randomString(JOIN_CODE_LENGTH);
}

/**
 * Find the session with the highest id and return id + 1
 * If there are no sessions, return 0.
 * Mod INT_MAX to prevent overflow.
 */

int SessionManager::getNextId() const
{
  int maxId = 0;

  for (const auto &sessionPair : sessions)
  {
    if (sessionPair.first > maxId)
    {
      maxId = sessionPair.first;
    }
  }

  return (maxId + 1) % INT_MAX;
}

Session &SessionManager::newSession(const GameData &gameData)
{
  Session session(getNextId(), gameData, generateJoinCode());
  sessions[session.getId()] = session;
  return sessions[session.getId()];
}

std::expected<Session*, std::string> SessionManager::createSession(uintptr_t playerID, const GameData &gameData)
{
  //Player cannot be in another session if creating a new one
  if (isPlayerInAnySession(playerID))
  {
    return std::unexpected("Player is already in another session");
  } else {
      Session& session = newSession(gameData);
      return &session;
  }
}

std::expected<void, std::string> SessionManager::destroySession(int sessionId)
{
  auto it = sessions.find(sessionId);
  if (it != sessions.end())
  {
    sessions.erase(it);
    return {};
  } else
  {
    return std::unexpected("Session not found");
  }
}

std::expected<Session*, std::string> SessionManager::getSession(int sessionId)
{
  auto it = sessions.find(sessionId);
  if (it != sessions.end())
  {
    return &it->second; // Return a pointer to the session if it exists
  } else
  {
    return std::unexpected("Session not found");
  }
}

std::expected<Session*, std::string> SessionManager::getSession(const std::string &joinCode)
{
  for (auto &sessionPair : sessions)
  {
    if (sessionPair.second.getJoinCode() == joinCode)
    {
      return &sessionPair.second; // Return a pointer to the session if the join code matches
    }
  }
  return std::unexpected("Session with specified join code not found"); // Throw unexpected if no matching session found
}

/**
 *
 */
std::expected<Session*, std::string> SessionManager::findSessionByPlayer(uintptr_t playerID)
{
  for (auto &sessionPair : sessions)
  {
    const auto &players = sessionPair.second.getPlayers();
    for (const auto &player : players)
    {
      if (player.getId() == playerID)
      {
        return &sessionPair.second; // Return a pointer to the session if the player ID matches
      }
    }
  }
  return std::unexpected("Session with player ID not found"); // return unexpected if session with playerID not found
}

bool SessionManager::isPlayerInAnySession(uintptr_t playerID)
{
  return findSessionByPlayer(playerID).has_value(); // if findSessionByPlayer returns unexpected
}

bool SessionManager::isSessionExists(int sessionId)
{
  return getSession(sessionId).has_value();
}

bool SessionManager::isSessionExists(const std::string &joinCode)
{
  return getSession(joinCode).has_value();
}

bool SessionManager::isPlayerInSession(const Session &session, uintptr_t playerID)
{
  const auto &players = session.getPlayers();
  for (const auto &player : players)
  {
    if (player.getId() == playerID)
    {
      return true;
    }
  }
  return false;
}

std::expected<Session*, std::string> SessionManager::addPlayerToSession(const std::string &joinCode, const Connection &connection)
{
  // Get session
  auto sessionResult = getSession(joinCode);
  if (sessionResult.has_value())
  {
    Session *session = sessionResult.value();

    // Check if player is already in this session
    if (isPlayerInSession(*session, connection.id))
    {
      return std::unexpected("Player is already in this session");
    }

    // Check if player is already in any other session
    if (isPlayerInAnySession(connection.id))
    {
      return std::unexpected("Player is already in another session");
    }

    // Add player to session
    session->addPlayer(connection);
    return session;
  }
  else
  {
    return std::unexpected(sessionResult.error()); // return unknown error
  }
}