#pragma once

#include "chat/chat.h"
#include <memory>
#include <vector>

class User;

/**
 * @brief Class for storing a user's chat list and their read states.
 */
class UserChatList {
private:
  std::weak_ptr<User> _owner;                 ///< Owner of the chat list (user).
  std::vector<std::weak_ptr<Chat>> _chatList; ///< List of user's chats.

public:
  /**
   * @brief Constructor for the user's chat list.
   * @param owner Shared pointer to the user who owns the chat list.
   */
  UserChatList(const std::shared_ptr<User> &owner);

  /**
   * @brief Default destructor.
   */
  ~UserChatList() = default;

  /**
   * @brief Gets the owner of the chat list.
   * @return Weak pointer to the user who owns the chat list.
   */
  std::weak_ptr<User> getOwner() const;

  /**
   * @brief Gets the list of chats for the user.
   * @return Vector of weak pointers to chats.
   */
  std::vector<std::weak_ptr<Chat>> getChatFromList() const;

  /**
   * @brief Adds a chat to the user's chat list.
   * @param chat Weak pointer to the chat to add.
   */
  void addChat(const std::weak_ptr<Chat> &chat);

  /**
   * @brief Delete a chat to the user's chat list.
   * @param chat Weak pointer to the chat to add.
   */
  void deleteChatFromList(const std::weak_ptr<Chat> &chat);

  // --- Дополнительные методы ---
};