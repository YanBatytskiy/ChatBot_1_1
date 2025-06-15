#include "user/user.h"
#include "exception/validation_exception.h"
#include "user/user_chat_list.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>

/**
 * @brief Constructor for User.
 * @param login User's login.
 * @param userName User's display name.
 * @param password User's password.
 */
User::User(const std::string &login, const std::string &userName, const std::string &password)
    : _login(login), _userName(userName), _password(password) {}

/**
 * @brief Assigns a chat list to the user.
 * @param userChats Shared pointer to the user's chat list.
 */
void User::createChatList(const std::shared_ptr<UserChatList> &userChats) { _userChats = userChats; }

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
std::string User::getPassword() const { return _password; }

/**
 * @brief Gets the user's chat list.
 * @return Shared pointer to the user's chat list.
 */
std::shared_ptr<UserChatList> User::getUserChatList() const { return _userChats; }

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
void User::setPassword(const std::string &password) { _password = password; }

/**
 * @brief Checks if the provided password matches the user's password.
 * @param password The password to check.
 * @return True if the password matches, false otherwise.
 */
bool User::checkPassword(const std::string &password) const { return (password == _password); }

/**
 * @brief Checks if the provided login matches the user's login.
 * @param login The login to check.
 * @return True if the login matches, false otherwise.
 */
bool User::checkLogin(const std::string &login) const { return (_login == login); }

/**
 * @brief Displays the user's data.
 * @details Prints the user's name, login, and password.
 */
void User::showUserData() const {
  std::cout << "Name: " << _userName << ", Login: " << _login << ", Password: " << _password << std::endl;
}

/**
 * @brief Prints the user's chat list.
 * @param user Shared pointer to the user whose chat list is to be printed.
 * @throws UnknownException If the message vector of a chat is empty.
 * @details Displays each chat with participant details, last message timestamp, and unread message count.
 * @note Needs to handle deleted users in the list and display unread message counts (marked as TODO).
 */
void User::printChatList(const std::shared_ptr<User> &user) const {
  // ДОДЕЛАТЬ ВЫВОД УДАЛЕННОГО ПОЛЬЗОВАТЕЛЯ В СПИСКЕ а также количество новых сообщений в списке

  std::string date_stamp;

  // достаем чатлист
  const auto &chatList = user->getUserChatList()->getChatFromList();

  if (chatList.empty()) {
    std::cout << "У пользователя " << user->_userName << " нет чатов." << std::endl;
    return;
  }
  std::cout << std::endl
            << "Всего чатов = " << user->getUserChatList()->getChatCount() << ". Список чатов пользователя "
            << user->_userName << " :" << std::endl;

  std::size_t index = 1;
  std::size_t activeUserMessageCount;
  std::size_t totalMessages;

  // перебираем чаты в списке
  for (const auto &weakChat : chatList) {
    if (auto chat_ptr = weakChat.lock()) {

      std::cout << std::endl;
      std::cout << index << ". ";

      const auto &messages = chat_ptr->getMessages();
      try {
        if (!messages.empty()) {
          totalMessages = messages.size();
          date_stamp = messages.back()->getTimeStamp();
        } else
          throw UnknownException("Вектор сообщений пуст. User::printChatList");
      } catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << std::endl;
      }
      // перебираем участников чата
      std::cout << "Имя/Логин: ";
      for (const auto &participant : chat_ptr->getParticipants()) {
        auto user_ptr = participant._user.lock();
        if (user_ptr) {
          if (user_ptr != user) {
            std::cout << user_ptr->getUserName() << "/" << user_ptr->getLogin() << "; ";
          } else {
            activeUserMessageCount = participant._lastReadMessageIndex;
          };
        } else {
          std::cout << "удал. пользоыватель";
        }
      }
    } else {
        std::cout << "Чат удален." << std::endl;
    }
    ++index;

    // выводим на печать дату и время последнего сообщения
    std::cout << "Последнее сообщение - " << date_stamp << ". ";

    // вывод на печать количества новых сообщений
    if (totalMessages > activeUserMessageCount)
      std::cout << "новых сообщений - " << totalMessages - activeUserMessageCount;
  }

  std::cout << std::endl;
}