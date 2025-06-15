#include "menu/1_registration.h"
#include "exception/login_exception.h"
#include "exception/validation_exception.h"
#include "system/chat_system.h"
#include "user/user.h"
#include "user/user_chat_list.h"
#include <algorithm>
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
  if (inputData.length() < contentLengthMin || inputData.length() > contentLengthMax)
    throw InvalidQuantityCharacterException();

  bool isCapital = false, isNumber = false;

  for (char ch : inputData) {
    if (!std::isalnum(ch))
      throw InvalidCharacterException(ch);

    if (std::isdigit(ch))
      isNumber = true;

    if (std::isupper(ch))
      isCapital = true;
  }

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
 * @param userData Structure for storing/comparing input data.
 * @param newUserData True if creating a new user.
 * @param chatSystem Reference to the chat system.
 * @return Validated input string.
 * @throws EmptyInputException If input is empty.
 */
std::string inputDataValidation(const std::string &prompt, std::size_t dataLengthMin, std::size_t dataLengthMax,
                                bool isPassword, UserData userData, bool newUserData, const ChatSystem &chatSystem) {
  std::string inputData;

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
  const auto &users = chatSystem.getUsers();
  auto it = std::find_if(users.begin(), users.end(),
                         [&](const std::shared_ptr<User> &u) { return u->getLogin() == userLogin; });
  return (it != users.end()) ? *it : nullptr;
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
 * @param userData Structure containing login and password.
 * @param chatSystem Reference to the chat system.
 * @return True if the password is valid, false otherwise.
 */
bool checkPasswordValidForUser(const UserData &userData, const ChatSystem &chatSystem) {
  auto user = findUserbyLogin(userData._login, chatSystem);
  return user && userData._password == user->getPassword();
}

/**
 * @brief Prompts and validates a new user login.
 * @param userData Structure to store the validated login.
 * @param chatSystem Reference to the chat system for uniqueness checks.
 */
void inputNewLogin(UserData &userData, const ChatSystem &chatSystem) {
  while (true) {
    std::size_t dataLengthMin = 5;
    std::size_t dataLengthMax = 15;
    std::string prompt;

    prompt = "Введите новый Логин либо 0 для выхода в предыдущее меню. Логин не менее " +
             std::to_string(dataLengthMin) + " символов и не более " + std::to_string(dataLengthMax) +
             " символов (можно использовать только латинские буквы и цифры) - ";

    std::string newLogin = inputDataValidation(prompt, dataLengthMin, dataLengthMax, false, userData, true, chatSystem);
    if (newLogin == "0")
      return;
    if (checkLoginExists(newLogin, chatSystem)) {
      std::cerr << "Логин уже занят.\n";
      continue;
    }

    userData._login = newLogin;
    return;
  }
}

/**
 * @brief Prompts and validates a new user password.
 * @param userData Structure to store the validated password.
 * @param chatSystem Reference to the chat system.
 */
void inputNewPassword(UserData &userData, const ChatSystem &chatSystem) {
  std::size_t dataLengthMin = 5;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите новый Пароль либо 0 для выхода в предыдущее меню. Пароль не менее " +
           std::to_string(dataLengthMin) + " символов и не более " + std::to_string(dataLengthMax) +
           " символов обязательно использовать минимум одну заглавную букву и одну цифру (можно использовать только "
           "латинские буквы и цифры) - ";

  std::string newPassword = inputDataValidation(prompt, dataLengthMin, dataLengthMax, true, userData, true, chatSystem);
  if (newPassword != "0")
    userData._password = newPassword;
}

/**
 * @brief Prompts and validates a new user display name.
 * @param userData Structure to store the validated name.
 * @param chatSystem Reference to the chat system.
 */
void inputNewName(UserData &userData, const ChatSystem &chatSystem) {
  std::size_t dataLengthMin = 3;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите желаемое Имя для отображения, не менее " + std::to_string(dataLengthMin) + "символов и не более " +
           std::to_string(dataLengthMax) + " символов, (можно использовать только латинские буквы и цифры) - ";

  std::string newName = inputDataValidation(prompt, dataLengthMin, dataLengthMax, false, userData, true, chatSystem);
  if (newName != "0")
    userData._name = newName;
}

/**
 * @brief Performs the full registration process for a new user.
 * @param chatSystem Reference to the chat system.
 * @details Handles input and validation of login, password, and name, then adds the user to the system.
 */
void userRegistration(ChatSystem &chatSystem) {
  std::cout << "Регистрация нового пользователя." << std::endl;
  UserData userData;

  inputNewLogin(userData, chatSystem);
  if (userData._login.empty())
    return;

  inputNewPassword(userData, chatSystem);
  if (userData._password.empty())
    return;

  inputNewName(userData, chatSystem);
  if (userData._name.empty())
    return;

  auto newUser = std::make_shared<User>(userData._login, userData._name, userData._password);
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
  UserData userData;

  // Login validation
  while (true) {
    try {
      userData._login = inputDataValidation("Введите логин или 0 для выхода:", 0, 0, false, userData, false,
                                            chatSystem);
      if (userData._login == "0")
        return false;

      if (!checkLoginExists(userData._login, chatSystem))
        throw UserNotFoundException();
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
    break;
  }

  auto user = findUserbyLogin(userData._login, chatSystem);
  chatSystem.setActiveUser(user);

  // Password validation
  while (true) {
    try {
      userData._password = inputDataValidation("Введите пароль (или 0 для выхода):", 0, 0, true, userData, false,
                                               chatSystem);
      if (userData._password == "0")
        return false;

      if (!checkPasswordValidForUser(userData, chatSystem))
        throw IncorrectPasswordException();

      auto user = findUserbyLogin(userData._login, chatSystem);
      chatSystem.setActiveUser(user);
      return true;
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
  }
}
