#pragma once
#include "chat/chat.h"
#include "system/dto_struct.h"
#include "system/id_generator.h"
#include "user/user.h"
#include <cstddef>
#include <memory>
#include <vector>

/**
 * @brief Manages users, chats, and the active user in the chat system.
 */
class ChatSystem {
private:
  std::vector<std::shared_ptr<User>> _users; ///< List of users in the system.
  std::vector<std::shared_ptr<Chat>> _chats; ///< List of chats in the system.
  std::shared_ptr<User> _activeUser;         ///< Current active user.
  std::unordered_map<std::string, std::shared_ptr<User>> _loginUserMap;
  std::unordered_map<std::size_t, std::shared_ptr<Chat>> _chatIdChatMap;
  idChatManager _idChatManager;
  idMessageManager _idMessageManager;

public:
  /**
   * @brief Default constructor for ChatSystem.
   */
  ChatSystem();

  /**
   * @brief Default destructor.
   */
  ~ChatSystem() = default;

  std::size_t getNewChatId();

  std::size_t getNewMessageId();

  std::shared_ptr<Chat> getChatById(std::size_t chatId) const;

  /**
s thChatMessagesf users.
   * @return Const reference to the vector of users.
   */
  const std::vector<std::shared_ptr<User>> &getUsers() const;

  /**
   * @brief Gets the list of chats.
   * @return Const reference to the vector of chats.
   */
  const std::vector<std::shared_ptr<Chat>> &getChats() const;

  /**
   * @brief Gets the active user.
   * @return Const reference to the active user.
   */
  const std::shared_ptr<User> &getActiveUser() const;

  /**
   * @brief Gets the login user map.
   * @return Const reference to the unordered map.
   */
  const std::unordered_map<std::string, std::shared_ptr<User>> &getLoginUserMap() const;

  void releaseChatId(std::size_t chatId);

  void releaseMessageId(std::size_t messageId);

  /**
   * @brief Sets the active user.
   * @param user Shared pointer to the user to set as active.
   */
  void setActiveUser(const std::shared_ptr<User> &user);

  /**
   * @brief Adds a user to the system.
   * @param user Shared pointer to the user to add.
   */
  void addUser(const std::shared_ptr<User> &user);

  /**
   * @brief Adds a chat to the system.
   * @param chat Shared pointer to the chat to add.
   */
  void addChat(const std::shared_ptr<Chat> &chat);

  /**
   * @brief Removes a user from the system.
   * @param user Shared pointer to the user to remove.
   */
  void eraseUser(const std::shared_ptr<User> &user);

  /**
   * @brief Removes a chat from the system.
   * @param chat Shared pointer to the chat to remove.
   */
  void eraseChat(const std::shared_ptr<Chat> &chat);

  /**
   * @brief Displays the list of users.
   * @param showActiveUser True to include the active user in the list.
   * @return The number of users displayed.
   */
  std::size_t showUserList(const bool showActiveUser); // вывод на экрын списка пользователей

  /**
   * @brief Finds users matching a search string.
   * @param users Vector to store found users.
   * @param textToFind Search string to match against user names or logins.
   */
  void findUserByTextPart(std::vector<std::shared_ptr<User>> &users,
                          const std::string &textToFind); // поиск пользователя
};
