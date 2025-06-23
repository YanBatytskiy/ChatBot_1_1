#include "id_generator.h"

/**
 * @brief Returns the next available chat ID.
 *
 * If there are no released IDs available, returns the next sequential ID.
 * Otherwise, reuses the smallest released ID.
 *
 * @return A unique chat ID.
 */
std::size_t idChatManager::getNextChatId() {
  if (_freeChatId.empty())
    return _nextChatId++;
  else {
    std::size_t value = *_freeChatId.begin();
    _freeChatId.erase(_freeChatId.begin());
    return value;
  }
}

/**
 * @brief Releases a chat ID back into the pool of available IDs.
 *
 * If the given ID is less than the current sequential ID counter,
 * it is inserted into the set of reusable IDs.
 *
 * @param chatId The chat ID to be released.
 */
void idChatManager::releaseChatId(std::size_t &chatId) {
  if (chatId < _nextChatId)
    _freeChatId.insert(chatId);
}

/**
 * @brief Returns the next available message ID.
 *
 * If there are no released IDs available, returns the next sequential ID.
 * Otherwise, reuses the smallest released ID.
 *
 * @return A unique message ID.
 */
std::size_t idMessageManager::getNextMessageId() {
  if (_freeMessageId.empty())
    return _nextMessageId++;
  else {
    std::size_t value = *_freeMessageId.begin();
    _freeMessageId.erase(_freeMessageId.begin());
    return value;
  }
}

/**
 * @brief Releases a message ID back into the pool of available IDs.
 *
 * If the given ID is less than the current sequential ID counter,
 * it is inserted into the set of reusable IDs.
 *
 * @param messageId The message ID to be released.
 */
void idMessageManager::releaseMessageId(std::size_t &messageId) {
  if (messageId < _nextMessageId)
    _freeMessageId.insert(messageId);
}
