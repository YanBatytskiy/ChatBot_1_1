#pragma once
#include <set>

/**
 * @brief Manages unique identifiers for chats.
 *
 * Provides sequential or recycled chat IDs. If an ID has been released,
 * it will be reused before allocating a new sequential one.
 */
class idChatManager {
private:
  std::set<std::size_t> _freeChatId; ///< Set of released chat IDs available for reuse.
  std::size_t _nextChatId = 1;       ///< Next available sequential chat ID.

public:
  /**
   * @brief Retrieves the next available chat ID.
   *
   * Returns the smallest ID from the released pool if available.
   * Otherwise, returns the current sequential ID and increments it.
   *
   * @return A unique chat ID.
   */
  std::size_t getNextChatId();

  /**
   * @brief Releases a chat ID for future reuse.
   *
   * Adds the specified ID to the pool of available chat IDs.
   *
   * @param chatId The chat ID to release.
   */
  void releaseChatId(std::size_t &chatId);
};

/**
 * @brief Manages unique identifiers for messages.
 *
 * Same mechanism as idChatManager but used for message ID tracking.
 */
class idMessageManager {
private:
  std::set<std::size_t> _freeMessageId; ///< Set of released message IDs available for reuse.
  std::size_t _nextMessageId = 1;       ///< Next available sequential message ID.

public:
  /**
   * @brief Retrieves the next available message ID.
   *
   * Returns the smallest ID from the released pool if available.
   * Otherwise, returns the current sequential ID and increments it.
   *
   * @return A unique message ID.
   */
  std::size_t getNextMessageId();

  /**
   * @brief Releases a message ID for future reuse.
   *
   * Adds the specified ID to the pool of available message IDs.
   *
   * @param messageId The message ID to release.
   */
  void releaseMessageId(std::size_t &messageId);
};
