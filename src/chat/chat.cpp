#include "chat/chat.h"
#include "exception/validation_exception.h"
#include <algorithm>
#include <iostream>

/**
 * @brief Adds chatId to the chat.
 */
void Chat::addChatId(std::size_t chatId) { _chatId = chatId; };

/**
 * @brief Adds a new participant to the chat.
 * @param user Shared pointer to the user to be added.
 * @details Creates a Participant struct, initializes it with the user, sets
 * last read message index to 0, and adds it to the participants vector.
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
 * @details Appends the message to the messages vector.
 */
void Chat::addMessage(const std::shared_ptr<Message> &message) { _messages.push_back(message); }

/**
 * @brief Sets the deletedFromChat status for a specific user.
 * @param user Shared pointer to the user.
 * @param deletedFromChat Boolean indicating if the user is deleted from the
 * chat.
 * @throws UserNotInListException If the user is not found in the participants
 * list.
 * @details Finds the participant and updates their deletedFromChat status.
 * Prints error if user is not found.
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
 * @brief Retrieves the chatId.
 */
const std::size_t &Chat::getChatId() const { return _chatId; };

/**
 * @brief Retrieves the list of messages in the chat.
 * @return Constant reference to the vector of messages.
 */
const std::vector<std::shared_ptr<Message>> &Chat::getMessages() const { return _messages; }

/**
 * @brief Retrieves the list of participants in the chat.
 * @return Constant reference to the vector of participants.
 */
const std::vector<Participant> &Chat::getParticipants() const { return _participants; }

/**
 * @brief Gets the index of the last message read by a specific user.
 * @param user Shared pointer to the user.
 * @return The index of the last read message, or 0 if user is not found.
 * @throws UserNotInListException If the user is not found in the participants
 * list.
 * @details Finds the participant and returns their last read message index.
 * Prints error and returns 0 if user is not found.
 */

std::size_t Chat::getLastReadMessageIndex(const std::shared_ptr<User> &user) const {

  auto it = _lastReadMessageMap.find(user);
  try {
    if (it != _lastReadMessageMap.end()) {
      return it->second;
    } else
      throw UserNotInListException();
  } catch (const ValidationException &ex) {
    std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
    return 0;
  }
}

/**
 * @brief Checks if a user has been deleted from the chat.
 * @param user Shared pointer to the user.
 * @return True if the user is deleted from the chat, false otherwise, or true
 * if user is not found.
 * @throws UserNotInListException If the user is not found in the participants
 * list.
 * @details Finds the participant and returns their deletedFromChat status.
 * Prints error and returns true if user is not found.
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
  } catch (const ValidationException &ex) {
    std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
    return true;
  }
}

/**
 * @brief Prints the full chat for a specific user.
 * @param currentUser Shared pointer to the user viewing the chat.
 * @details Iterates through messages and calls their printMessage method.
 * Prints "Cообщуний нет." if the chat is empty.
 * @note The parameter name in the header (¤tUser) is a typo and should be
 * corrected to currentUser.
 */
void Chat::printChat(const std::shared_ptr<User> &currentUser) {

  if (!_messages.empty()) {
    auto messageCount = _messages.size();

    // получаем количество неппрочитанных
    auto unReadCount = this->getLastReadMessageIndex(currentUser);

    std::cout << std::endl
              << "Вот твой чат, GUID: " << this->getChatId() << ". В нем всего " << messageCount << " сообщения(ий). ";
    std::cout << "\033[32m"; // red
    std::cout << "Из них непрочитанных - " << messageCount - unReadCount << std::endl;
    std::cout << "\033[0m";

    // выводим список участников чата кроме активного юзера
    std::cout << std::endl << "Участники чата Имя/Логин: " << std::endl;
    // перебираем участников чата
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

    // печатаем сами сообщения
    for (const auto &message : _messages)
      //   {
      message->printMessage(currentUser);
    // }
  } else
    std::cout << "Cообщений нет." << std::endl;
}

/**
 * @brief Updates the last read message index for a specific user.
 * @param user Shared pointer to the user.
 * @param newLastReadMessageIndex The new index of the last read message.
 * @throws UserNotInListException If the user is not found in the participants
 * list.
 * @details Finds the participant by login and updates their last read message
 * index. Prints error if user is not found.
 */

void Chat::updateLastReadMessageIndex(const std::shared_ptr<User> &user, std::size_t newLastReadMessageIndex) {

  _lastReadMessageMap[user] = newLastReadMessageIndex;
}

/**
 * @brief Removes a participant from the chat (to be implemented).
 * @param user Shared pointer to the user to be removed.
 * @note This method is currently commented out and requires implementation.
 */
// void Chat::removeParticipant(const std::shared_ptr<User> &user) {
// }