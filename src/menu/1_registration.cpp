#include "1_registration.h"
#include "exception/login_exception.h"
#include "exception/validation_exception.h"
#include "client-server/chat_system.h"
#include "system/picosha2.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>

/**
 * @brief Validates login or password input against specified constraints.
 * @param inputData The input string to validate.
 * @param contentLengthMin Minimum allowed length.
 * @param contentLengthMax Maximum allowed length.
 * @param isPassword True if validating a password, false for login.
 * @return True if the input is valid.
 * @throws InvalidQuantityCharacterException If the length is out of bounds.
 * @throws InvalidCharacterException If non-alphanumeric characters are found.
 * @throws NonCapitalCharacterException If password lacks a capital letter.
 * @throws NonDigitalCharacterException If password lacks a digit.
 */
bool checkNewDataInputForLimits(const std::string &inputData, std::size_t contentLengthMin,
                                std::size_t contentLengthMax, bool isPassword) {

  bool isCapital = false, isNumber = false;
  std::size_t utf8SymbolCount = 0;

  for (std::size_t i = 0; i < inputData.size();) {

    std::size_t charLen = getUtf8CharLen(static_cast<unsigned char>(inputData[i]));

    if (i + charLen > inputData.size())
      throw InvalidCharacterException("");

    std::string utf8Char = inputData.substr(i, charLen);

    ++utf8SymbolCount;

    if (charLen == 1) {
      char ch = utf8Char[0];
      if (std::isdigit(ch))
        isNumber = true;

      if (std::isupper(ch))
        isCapital = true;
    } // if
    else
      throw InvalidCharacterException("");

    i += charLen;

  } // for i

  if (utf8SymbolCount < contentLengthMin || utf8SymbolCount > contentLengthMax)
    throw InvalidQuantityCharacterException();

  if (isPassword) {
    if (!isCapital)
      throw NonCapitalCharacterException();
    if (!isNumber)
      throw NonDigitalCharacterException();
  }

  return true;
}

/**
 * @brief Validates user input with retry attempts.
 * @param prompt Message displayed to the user.
 * @param dataLengthMin Minimum length of the input.
 * @param dataLengthMax Maximum length of the input.
 * @param isPassword True if input is a password.
 * @param newUserData True if creating a new user.
 * @param chatSystem Reference to the chat system.
 * @return Validated input string.
 * @throws EmptyInputException If input is empty.
 */
std::string inputDataValidation(const std::string &prompt, std::size_t dataLengthMin, std::size_t dataLengthMax,
                                bool isPassword, bool newUserData, const ChatSystem &chatSystem) {
  std::string inputData;

  // доделать проверку на utf-8 символы английского языка
  while (true) {
    std::cout << prompt << std::endl;
    std::getline(std::cin, inputData);
    try {
      if (inputData.empty())
        throw EmptyInputException();

      if (inputData == "0")
        return inputData;

      if (newUserData)
        checkNewDataInputForLimits(inputData, dataLengthMin, dataLengthMax, isPassword);

      return inputData;
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
  }
}

/**
 * @brief Finds a user by login in the chat system.
 * @param userLogin The login to search for.
 * @param chatSystem Reference to the chat system.
 * @return Shared pointer to the found user, or nullptr if not found.
 */
std::shared_ptr<User> findUserbyLogin(const std::string &userLogin, const ChatSystem &chatSystem) {
  //   const auto &users = chatSystem.getUsers();
  //   auto it = std::find_if(users.begin(), users.end(),
  //                          [&](const std::shared_ptr<User> &u) {
  //                            return u->getLogin() == userLogin;
  //                          });
  //   return (it != users.end()) ? *it : nullptr;

  auto it = chatSystem.getLoginUserMap().find(userLogin);
  if (it == chatSystem.getLoginUserMap().end())
    return nullptr;
  else
    return it->second;
}

/**
 * @brief Checks if a user with the specified login exists.
 * @param login The login to check.
 * @param chatSystem Reference to the chat system.
 * @return Shared pointer to the user if found, or nullptr if not found.
 */
const std::shared_ptr<User> checkLoginExists(const std::string &login, const ChatSystem &chatSystem) {
  for (const auto &user : chatSystem.getUsers()) {
    if (user->getLogin() == login)
      return user;
  }
  return nullptr;
}

/**
 * @brief Verifies if the provided password matches the user's password.
 * @param chatSystem Reference to the chat system.
 * @return True if the password is valid, false otherwise.
 */
bool checkPasswordValidForUser(const std::string &userPassword, const std::string &userLogin,
                               const ChatSystem &chatSystem) {

  auto user = findUserbyLogin(userLogin, chatSystem);

  std::string passwordHash = picosha2::hash256_hex_string(userPassword);

  return passwordHash == user->getPassword();
}

/**
 * @brief Prompts and validates a new user login.
 * @param chatSystem Reference to the chat system for uniqueness checks.
 */
std::string inputNewLogin(const ChatSystem &chatSystem) {
  while (true) {
    std::size_t dataLengthMin = 5;
    std::size_t dataLengthMax = 15;
    std::string prompt;

    prompt = "Введите новый Логин либо 0 для выхода в предыдущее меню. Логин "
             "не менее " +
             std::to_string(dataLengthMin) + " символов и не более " + std::to_string(dataLengthMax) +
             " символов (можно использовать только латинские буквы и цифры) - ";

    std::string newLogin = inputDataValidation(prompt, dataLengthMin, dataLengthMax, false, true, chatSystem);

    if (newLogin == "0") {
      return newLogin;
      newLogin.clear();
    }

    if (checkLoginExists(newLogin, chatSystem)) {
      std::cerr << "Логин уже занят.\n";
      continue;
    }

    return newLogin;
  }
}

/**
 * @brief Prompts and validates a new user password.
 * @param chatSystem Reference to the chat system.
 */
std::string inputNewPassword(const ChatSystem &chatSystem) {

  std::size_t dataLengthMin = 5;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите новый Пароль либо 0 для выхода в предыдущее меню. Пароль "
           "не менее " +
           std::to_string(dataLengthMin) + " символов и не более " + std::to_string(dataLengthMax) +
           " символов обязательно использовать минимум одну заглавную букву и "
           "одну цифру (можно использовать только "
           "латинские буквы и цифры) - ";

  std::string newPassword = inputDataValidation(prompt, dataLengthMin, dataLengthMax, true, true, chatSystem);

  if (newPassword == "0")
    newPassword.clear();
  else
    newPassword = picosha2::hash256_hex_string(newPassword);

  return newPassword;
}

/**
 * @brief Prompts and validates a new user display name.
 * @param chatSystem Reference to the chat system.
 */
std::string inputNewName(const ChatSystem &chatSystem) {
  std::size_t dataLengthMin = 3;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите желаемое Имя для отображениялибо 0 для выхода в предыдущее "
           "меню. Не менее " +
           std::to_string(dataLengthMin) + "символов и не более " + std::to_string(dataLengthMax) +
           " символов, (можно использовать только латинские буквы и цифры) - ";

  std::string newName = inputDataValidation(prompt, dataLengthMin, dataLengthMax, false, true, chatSystem);

  if (newName == "0")
    newName.clear();

  return newName;
}

/**
 * @brief Performs the full registration process for a new user.
 * @param chatSystem Reference to the chat system.
 * @details Handles input and validation of login, password, and name, then adds
 * the user to the system.
 */
void userRegistration(ChatSystem &chatSystem) {
  std::cout << "Регистрация нового пользователя." << std::endl;
  UserData userData;

  std::string newLogin = inputNewLogin(chatSystem);
  if (newLogin.empty())
    return;

  std::string newPassword = inputNewPassword(chatSystem);
  if (newPassword.empty())
    return;

  std::string newName = inputNewName(chatSystem);
  if (newName.empty())
    return;

  auto newUser = std::make_shared<User>(UserData(newLogin, newName, newPassword, "...@gmail.com", "+111"));

  chatSystem.addUser(newUser);
  chatSystem.setActiveUser(newUser);
  newUser->showUserData();
  newUser->createChatList(std::make_shared<UserChatList>(newUser));
}

/**
 * @brief Authenticates a user in the chat system.
 * @param chatSystem Reference to the chat system.
 * @return True if login is successful, false if login fails.
 * @throws UserNotFoundException If the login does not exist.
 * @throws IncorrectPasswordException If the password is incorrect.
 */
bool userLoginInsystem(ChatSystem &chatSystem) {
  UserData userDataForLogin;
  std::string userLogin = "";
  std::string userPassword = "";

  // Login validation
  while (true) {
    try {
      userLogin = inputDataValidation("Введите логин или 0 для выхода:", 0, 0, false, false, chatSystem);
      if (userLogin == "0")
        return false;

      if (!checkLoginExists(userLogin, chatSystem))
        throw UserNotFoundException();
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
    break;
  }

  auto user = findUserbyLogin(userLogin, chatSystem);
  chatSystem.setActiveUser(user);

  // Password validation
  while (true) {
    try {
      userPassword = inputDataValidation("Введите пароль (или 0 для выхода):", 0, 0, true, false, chatSystem);
      if (userPassword == "0")
        return false;

      if (!checkPasswordValidForUser(userPassword, userLogin, chatSystem))
        throw IncorrectPasswordException();

      auto user = findUserbyLogin(userLogin, chatSystem);
      chatSystem.setActiveUser(user);
      return true;
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
  }
}
