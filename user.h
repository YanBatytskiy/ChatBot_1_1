#pragma once

#include <string>

/**
 * @brief Class representing a user in the chat system.
 */
class User {
private:
  std::string _login;                       ///< User login.
  std::string _userName;                    ///< User's display name.
  std::string _passwordHash;                    ///< User passwordHash.

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
  std::string getPasswordHash() const;

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

};