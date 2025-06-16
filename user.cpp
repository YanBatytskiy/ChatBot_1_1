#include "user.h"
#include "picosha2.h"
#include <iostream>
#include <ostream>
/**
 * @brief Constructor for User.
 * @param login User's login.
 * @param userName User's display name.
 * @param password User's password.
 */
User::User(const std::string &login, const std::string &userName,
           const std::string &password)
    : _login(login), _userName(userName) {
  setPassword(password);
}

/**
 * @brief Gets the user's login.
 * @return The user's login string.
 */
std::string User::getLogin() const { return _login; }

/**
 * @brief Gets the user's display name.
 * @return The user's display name string.
 */
std::string User::getUserName() const { return _userName; }

/**
 * @brief Gets the user's password.
 * @return The user's password string.
 */
std::string User::getPasswordHash() const { return _passwordHash; }

/**
 * @brief Sets the user's login.
 * @param login The new login string.
 */
void User::setLogin(const std::string &login) { _login = login; }

/**
 * @brief Sets the user's display name.
 * @param userName The new display name string.
 */
void User::setUserName(const std::string &userName) { _userName = userName; }

/**
 * @brief Sets the user's password.
 * @param password The new password string.
 */
void User::setPassword(const std::string &password) {
  _passwordHash = picosha2::hash256_hex_string(password);
}

/**
 * @brief Checks if the provided password matches the user's password.
 * @param password The password to check.
 * @return True if the password matches, false otherwise.
 */
bool User::checkPassword(const std::string &password) const {
  return (_passwordHash == picosha2::hash256_hex_string(password));
}

/**
 * @brief Checks if the provided login matches the user's login.
 * @param login The login to check.
 * @return True if the login matches, false otherwise.
 */
bool User::checkLogin(const std::string &login) const {
  return (_login == login);
}

/**
 * @brief Displays the user's data.
 * @details Prints the user's name, login, and password.
 */
void User::showUserData() const {
  std::cout << "Name: " << _userName << ", Login: " << _login
            << ", PasswordHash: " << _passwordHash << std::endl;
}
