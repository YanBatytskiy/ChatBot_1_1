#include "chat_system.h"
#include "chat/chat.h"
#include "client-server/client_session.h"
#include "system/system_function.h"
#include <iostream>
#include <memory>

/**
 * @brief Default constructor for ChatSystem.
 */
ChatSystem::ChatSystem() {}

std::size_t ChatSystem::getNewChatId() { return _idChatManager.getNextChatId(); }

std::size_t ChatSystem::getNewMessageId() { return _idMessageManager.getNextMessageId(); }

std::shared_ptr<Chat> ChatSystem::getChatById(std::size_t chatId) const {
  auto it = _chatIdChatMap.find(chatId);
  if (it != _chatIdChatMap.end())
    return it->second;
  return nullptr;
}

/**
 * @brief Gets the list of users.
 * @return Const reference to the vector of users.
 */
const std::vector<std::shared_ptr<User>> &ChatSystem::getUsers() const { return _users; }

/**
 * @brief Gets the list of chats.
 * @return Const reference to the vector of chats.
 */
const std::vector<std::shared_ptr<Chat>> &ChatSystem::getChats() const { return _chats; }

/**
 * @brief Gets the active user.
 * @return Const reference to the active user.
 */
const std::shared_ptr<User> &ChatSystem::getActiveUser() const { return _activeUser; }

/**
 * @brief Gets the login user map.
 * @return Const reference to the unordered map.
 */
const std::unordered_map<std::string, std::shared_ptr<User>> &ChatSystem::getLoginUserMap() const {
  return _loginUserMap;
};

void ChatSystem::releaseChatId(std::size_t chatId) { _idChatManager.releaseChatId(chatId); }

void ChatSystem::releaseMessageId(std::size_t messageId) { _idMessageManager.releaseMessageId(messageId); };

/**
 * @brief Sets the active user.
 * @param user Shared pointer to the user to set as active.
 */
void ChatSystem::setActiveUser(const std::shared_ptr<User> &user) { _activeUser = user; }

/**
 * @brief Adds a user to the system.
 * @param user Shared pointer to the user to add.
 */
void ChatSystem::addUser(const std::shared_ptr<User> &user) {
  _users.push_back(user);
  _loginUserMap.insert({user->getLogin(), user});
}

/**
 * @brief Adds a chat to the system.
 * @param chat Shared pointer to the chat to add.
 */
void ChatSystem::addChat(const std::shared_ptr<Chat> &chat) {
  _chats.push_back(chat);
  std::size_t newChatId = getNewChatId();
  chat->addChatId(newChatId);
  _chatIdChatMap.insert({newChatId, chat});
}

/**
 * @brief Removes a user from the system (not implemented).
 * @param user Shared pointer to the user to remove.
 */
void ChatSystem::eraseUser(const std::shared_ptr<User> &user) {
  // доделать
}

/**
 * @brief Removes a chat from the system (not implemented).
 * @param chat Shared pointer to the chat to remove.
 */
void ChatSystem::eraseChat(const std::shared_ptr<Chat> &chat) {
  // доделать
}

/**
 * @brief Displays the list of users.
 * @param showActiveUser True to include the active user in the list.
 * @return The index of the active user in the list.
 * @details Prints user names and logins, excluding the active user if
 * showActiveUser is false.
 */
std::size_t ChatSystem::showUserList(const bool showActiveUser) { // вывод на экрын списка пользователей
  std::cout << "Список пользователей:" << std::endl;
  size_t index = 1;
  size_t returnIndex = std::string::npos;
  for (const auto &user : _users) {
    if (user == _activeUser) {
      returnIndex = index - 1;
    }

    if (!showActiveUser && user == _activeUser)
      continue;
    std::cout << index << ".  Имя - " << user->getUserName() << ", логин - " << user->getLogin() << ";" << std::endl;
    ++index;
  }
  return returnIndex;
}

/**
 * @brief Finds users matching a search string.
 * @param users Vector to store found users.
 * @param textToFind Search string to match against user names or logins.
 * @details Performs case-insensitive search for users, excluding the active
 * user.
 */
void ChatSystem::findUserByTextPart(std::vector<std::shared_ptr<User>> &foundUsers,
                                    const std::string &textToFind) { // поиск пользователя

  std::string textToFindLower = TextToLower(textToFind);

  // перебираем всех пользователей в векторе
  for (const auto &user : _users) {

    if (user == _activeUser)
      continue;

    std::string LowerLogin = TextToLower(user->getLogin());
    std::string LowerName = TextToLower(user->getUserName());

    if (LowerLogin.find(textToFindLower) != std::string::npos || LowerName.find(textToFindLower) != std::string::npos)
      foundUsers.push_back(user);
  }
}