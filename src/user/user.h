#pragma once

#include <memory>
#include <string>

class UserChatList;

/**
 * @brief Class representing a user in the chat system.
 */
class User {
private:
  std::string _login;                       ///< User login.
  std::string _userName;                    ///< User's display name.
  std::string _password;                    ///< User password.
  std::shared_ptr<UserChatList> _userChats; ///< User's chat list.

public:
  /**
   * @brief Constructor for User.
   * @param login User's login.
   * @param userName User's display name.
   * @param password User's password.
   */
  User(const std::string &login, const std::string &userName, const std::string &password);

  /**
   * @brief Default destructor.
   */
  ~User() = default;

  /**
   * @brief Assigns a chat list to the user.
   * @param chats Shared pointer to the user's chat list.
   */
  void createChatList(const std::shared_ptr<UserChatList> &chats);

  /**
   * @brief Gets the user's login.
   * @return The user's login string.
   */
  std::string getLogin() const;

  /**
   * @brief Gets the user's display name.
   * @return The user's display name string.
   */
  std::string getUserName() const;

  /**
   * @brief Gets the user's password.
   * @return The user's password string.
   */
  std::string getPassword() const;

  /**
   * @brief Gets the user's chat list.
   * @return Shared pointer to the user's chat list.
   */
  std::shared_ptr<UserChatList> getUserChatList() const;

  /**
   * @brief Sets the user's login.
   * @param login The new login string.
   */
  void setLogin(const std::string &login);

  /**
   * @brief Sets the user's display name.
   * @param userName The new display name string.
   */
  void setUserName(const std::string &userName);

  /**
   * @brief Sets the user's password.
   * @param password The new password string.
   */
  void setPassword(const std::string &password);

  /**
   * @brief Checks if the provided password matches the user's password.
   * @param password The password to check.
   * @return True if the password matches, false otherwise.
   */
  bool checkPassword(const std::string &password) const;

  /**
   * @brief Checks if the provided login matches the user's login.
   * @param login The login to check.
   * @return True if the login matches, false otherwise.
   */
  bool checkLogin(const std::string &login) const;

  /**
   * @brief Displays the user's data.
   * @details Prints the user's login, name, and other relevant information.
   */
  void showUserData() const;

  /**
   * @brief Prints the user's chat list.
   * @param user Shared pointer to the user whose chat list is to be printed.
   */
  void printChatList(const std::shared_ptr<User> &user) const;
};

