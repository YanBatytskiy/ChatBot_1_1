#include "chat/chat.h"
#include "exception/validation_exception.h"
#include <algorithm>
#include <iostream>

/**
 * @brief Sets the chat ID.
 * @param chatId Unique identifier for the chat.
 */
void Chat::addChatId(std::size_t chatId) { _chatId = chatId; };

/**
 * @brief Adds a new participant to the chat.
 * @param user Shared pointer to the user to be added.
 */
void Chat::addParticipant(const std::shared_ptr<User> &user) {
  Participant participant;
  participant._user = user;
  _participants.push_back(participant);
  updateLastReadMessageIndex(user, 0);
}

/**
 * @brief Adds a message to the chat.
 * @param message Shared pointer to the message to be added.
 */
void Chat::addMessage(const std::shared_ptr<Message> &message) {
  _messages.push_back(message);
}

/**
 * @brief Marks a user as deleted from the chat.
 * @param user Shared pointer to the user.
 */
void Chat::setDeletedFromChat(const std::shared_ptr<User> &user) {
  auto &participants = _participants;
  auto it = std::find_if(participants.begin(), participants.end(), [&user](const Participant &participant) {
    auto user_ptr = participant._user.lock();
    return user_ptr && (user_ptr == user);
  });
  try {
    if (it != participants.end())
      it->_deletedFromChat = true;
    else
      throw UserNotInListException();
  } catch (const ValidationException &ex) {
    std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
  }
}

/**
 * @brief Returns the chat ID.
 * @return Const reference to chat ID.
 */
const std::size_t &Chat::getChatId() const { return _chatId; };

/**
 * @brief Returns the list of messages in the chat.
 * @return Const reference to vector of shared pointers to messages.
 */
const std::vector<std::shared_ptr<Message>> &Chat::getMessages() const { return _messages; }

/**
 * @brief Returns the list of participants in the chat.
 * @return Const reference to vector of participants.
 */
const std::vector<Participant> &Chat::getParticipants() const { return _participants; }

/**
 * @brief Returns the last read message index for a user.
 * @param user Shared pointer to the user.
 * @return Index of the last read message. Returns 0 if user not found.
 */
std::size_t Chat::getLastReadMessageIndex(const std::shared_ptr<User> &user) const {
  auto it = _lastReadMessageMap.find(user);
  if (it != _lastReadMessageMap.end())
    return it->second;
  return 0;
}

/**
 * @brief Checks if a user is marked as deleted from the chat.
 * @param user Shared pointer to the user.
 * @return True if deleted, false if present. Returns true if user not found.
 */
bool Chat::getDeletedFromChat(const std::shared_ptr<User> &user) const {
  const auto &participants = _participants;
  auto it = std::find_if(participants.begin(), participants.end(), [&user](const Participant &participant) {
    auto user_ptr = participant._user.lock();
    return user_ptr && (user_ptr == user);
  });
  try {
    if (it != participants.end())
      return it->_deletedFromChat;
    else
      throw UserNotInListException();
  } catch (const UserNotInListException &ex) {
    std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
    return true;
  }
}

/**
 * @brief Prints the chat content for the current user.
 * @param currentUser Shared pointer to the user viewing the chat.
 */
void Chat::printChat(const std::shared_ptr<User> &currentUser) {
  if (!_messages.empty()) {
    auto messageCount = _messages.size();
    auto unReadCount = this->getLastReadMessageIndex(currentUser);

    std::cout << std::endl
              << "Вот твой чат, chatId: " << this->getChatId() << ". В нем всего " << messageCount << " сообщения(ий). ";
    std::cout << "\033[32m";
    std::cout << "Из них непрочитанных - " << messageCount - unReadCount << std::endl;
    std::cout << "\033[0m";

    std::cout << std::endl << "Участники чата Имя/Логин: " << std::endl;
    for (const auto &participant : this->getParticipants()) {
      auto user_ptr = participant._user.lock();
      if (user_ptr) {
        if (user_ptr != currentUser) {
          std::cout << user_ptr->getUserName() << "/" << user_ptr->getLogin() << "; ";
        };
      } else {
        std::cout << "удал. пользоыватель";
      }
    }

    std::cout << std::endl;

    for (const auto &message : _messages)
      message->printMessage(currentUser);
  } else
    std::cout << "Cообщений нет." << std::endl;
}

/**
 * @brief Updates the last read message index for a user.
 * @param user Shared pointer to the user.
 * @param newLastReadMessageIndex New index to store.
 */
void Chat::updateLastReadMessageIndex(const std::shared_ptr<User> &user, std::size_t newLastReadMessageIndex) {
  _lastReadMessageMap[user] = newLastReadMessageIndex;
}

// void Chat::removeParticipant(const std::shared_ptr<User> &user) {
// }
