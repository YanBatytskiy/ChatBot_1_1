#include "user.h"
#include "exception/validation_exception.h"
#include "user_chat_list.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>

/**  auto Alex2104_ptr = std::make_shared<User>("alex1980", "Sasha", "User01");

 * @brief Constructor for User.
 * @param login User's login.
 * @param userName User's display name.
 * @param passwordHash User's passwordHash.
 */
// User::User(const std::string &login, const std::string &userName,
//            const std::string &passwordHash)
//     : _login(login), getUserName()(userName),
//     _userData._passwordHash(passwordHash) {}

User::User(const UserData &userData) : _userData(userData) {}

/**
 * @brief Assigns a chat list to the user.
 * @param userChats Shared pointer to the user's chat list.
 */
void User::createChatList(const std::shared_ptr<UserChatList> &userChats) { _userChats = userChats; }

/**
 * @brief Gets the user's login.
 * @return The user's login string.
 */
std::string User::getLogin() const { return _userData._login; }

/**
 * @brief Gets the user's display name.
 * @return The user's display name string.
 */
std::string User::getUserName() const { return _userData._userName; }

/**
 * @brief Gets the user's passwordHash.
 * @return The user's passwordHash string.
 */
std::string User::getPassword() const { return _userData._passwordHash; }

/**
 * @brief Gets the user's email.
 * @return The user's Email string.
 */
std::string User::getEmail() const { return _userData._email; };

/**
 * @brief Gets the user's phone.
 * @return The user's phone string.
 */
std::string User::getPhone() const { return _userData._phone; };

/**
 * @brief Gets the user's chat list.
 * @return Shared pointer to the user's chat list.
 */
std::shared_ptr<UserChatList> User::getUserChatList() const { return _userChats; }

/**
 * @brief Sets the user's login.
 * @param login The new login string.
 */
void User::setLogin(const std::string &login) { _userData._login = login; }

/**
 * @brief Sets the user's display name.
 * @param userName The new display name string.
 */
void User::setUserName(const std::string &userName) { getUserName() = userName; }

/**
 * @brief Sets the user's passwordHash.
 * @param passwordHash The new passwordHash string.
 */
void User::setPassword(const std::string &passwordHash) { _userData._passwordHash = passwordHash; }

/**
 * @brief Sets the user's email.
 * @param email The new email string.
 */
void User::setEmail(const std::string &email) { _userData._email = email; };

/**
 * @brief Sets the user's phone.
 * @param phone The new phone string.
 */
void User::setPhone(const std::string &phone) { _userData._phone = phone; };

/**
 * @brief Checks if the provided passwordHash matches the user's passwordHash.
 * @param passwordHash The passwordHash to check.
 * @return True if the passwordHash matches, false otherwise.
 */
bool User::checkPassword(const std::string &passwordHash) const { return (passwordHash == getPassword()); }

/**
 * @brief Checks if the provided login matches the user's login.
 * @param login The login to check.
 * @return True if the login matches, false otherwise.
 */
bool User::checkLogin(const std::string &login) const { return (getLogin() == login); }

/**
 * @brief Displays the user's data.
 * @details Prints the user's name, login, and passwordHash.
 */
void User::showUserData() const { std::cout << "Name: " << getUserName() << ", Login: " << getLogin() << std::endl; }

/**
 * @brief Displays the user's data for init.
 * @details Prints the user's name, login, and passwordHash.
 */
void User::showUserDataInit() const { std::cout << "Name: " << getUserName() << ", Login: " << getLogin(); }

/**
 * @brief Prints the user's chat list.
 * @param user Shared pointer to the user whose chat list is to be printed.
 * @throws UnknownException If the message vector of a chat is empty.
 * @details Displays each chat with participant details, last message timestamp,
 * and unread message count.
 * @note Needs to handle deleted users in the list and display unread message
 * counts (marked as TODO).
 */
void User::printChatList(const std::shared_ptr<User> &user) const {
  // ДОДЕЛАТЬ ВЫВОД УДАЛЕННОГО ПОЛЬЗОВАТЕЛЯ В СПИСКЕ а также количество новых
  // сообщений в списке

  std::string date_stamp;

  // достаем чатлист
  const auto &chatList = user->getUserChatList()->getChatFromList();

  if (chatList.empty()) {
    std::cout << "У пользователя " << user->getUserName() << " нет чатов." << std::endl;
    return;
  }
  std::cout << std::endl
            << "Всего чатов = " << user->getUserChatList()->getChatCount() << ". Список чатов пользователя "
            << user->getUserName() << " :" << std::endl;

  std::size_t index = 1;
  std::size_t activeUserMessageCount = 0;
  std::size_t totalMessages = 0;

  // перебираем чаты в списке
  for (const auto &weakChat : chatList) {

    date_stamp.clear();
    totalMessages = 0;
    activeUserMessageCount = 0;

    if (auto chat_ptr = weakChat.lock()) {

      std::cout << std::endl;
      std::cout << index << ". GUID чата: " << chat_ptr->getChatId() << ", "; 

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
            try {
              activeUserMessageCount = chat_ptr->getLastReadMessageIndex(user_ptr);
            } // try
            catch (const UserNotInListException &) {
              activeUserMessageCount = 0;
            } // catch
          } // else if (user_ptr != user)
        } // if (user_ptr)
        else {
          std::cout << "удал. пользователь";
        }
      } // for participant
      // выводим на печать дату и время последнего сообщения
      std::cout << "Последнее сообщение - " << date_stamp << ". ";

      // вывод на печать количества новых сообщений
      if (totalMessages > activeUserMessageCount)
        std::cout << "новых сообщений - " << totalMessages - activeUserMessageCount;
      std::cout << std::endl;

    } // if auto chat_ptr
    else {
      std::cout << "Чат удален." << std::endl;
    }
    ++index;

  } // for (const auto &weakChat
}