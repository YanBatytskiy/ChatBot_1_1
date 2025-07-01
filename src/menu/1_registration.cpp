#include "1_registration.h"
#include "ChatBot/chat_system.h"
#include "exception/login_exception.h"
#include "exception/validation_exception.h"
#include "system/picosha2.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>

/**
 * @brief Validates a login or password string against defined constraints.
 * @param inputData The input string to validate.
 * @param contentLengthMin The minimum allowed length.
 * @param contentLengthMax The maximum allowed length.
 * @param isPassword Set to true if the input is a password, false if it's a login.
 * @return True if the input passes all validation checks.
 * @throws InvalidQuantityCharacterException If the length is outside the allowed range.
 * @throws InvalidCharacterException If non-ASCII or invalid characters are found.
 * @throws NonCapitalCharacterException If the password lacks an uppercase letter.
 * @throws NonDigitalCharacterException If the password lacks a numeric digit.
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
    } else
      throw InvalidCharacterException(""); // std::cout << "DEBUG NewUser:" << std::endl;
    // std::cout << "Login: " << user->getLogin() << std::endl;
    // std::cout << "Name: " << user->getUserName() << std::endl;
    // std::cout << "PassHash: " << user->getPassword() << std::endl;
    // std::cout << "Email: " << user->getEmail() << std::endl;
    // std::cout << "Phone: " << user->getPhone() << std::endl;

    i += charLen;
  }

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
 * @brief Prompts the user for input and validates it according to specified rules.
 * @param prompt The message shown to the user.
 * @param dataLengthMin The minimum length allowed for the input.
 * @param dataLengthMax The maximum length allowed for the input.
 * @param isPassword True if validating a password, false for login or name.
 * @param newUserData True if creating a new user.
 * @param chatSystem Reference to the chat system.
 * @return The validated input string.
 * @throws EmptyInputException If the input is empty.
 */
std::string inputDataValidation(const std::string &prompt, std::size_t dataLengthMin, std::size_t dataLengthMax,
                                bool isPassword, bool newUserData, const ChatSystem &chatSystem) {
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
 * @brief Searches for a user in the chat system by login.
 * @param userLogin The login to search for.
 * @param chatSystem Reference to the chat system.
 * @return A shared pointer to the user if found, or nullptr otherwise.
 */
std::shared_ptr<User> findUserbyLogin(const std::string &userLogin, const ChatSystem &chatSystem) {

  auto it = chatSystem.getLoginUserMap().find(userLogin);
  if (it == chatSystem.getLoginUserMap().end())
    return nullptr;
  else
    return it->second;
}

/**
 * @brief Checks if a login already exists in the system.
 * @param login The login to check.
 * @param chatSystem Reference to the chat system.
 * @return A shared pointer to the user if found, or nullptr otherwise.
 */
const std::shared_ptr<User> checkLoginExists(const std::string &login, const ChatSystem &chatSystem) {
  for (const auto &user : chatSystem.getUsers()) {
    if (user->getLogin() == login)
      return user;
  }
  return nullptr;
}

/**
 * @brief Checks whether the given password is valid for the specified user.
 * @param userPassword The password to verify.
 * @param userLogin The login of the user.
 * @param chatSystem Reference to the chat system.
 * @return True if the password matches the stored one; false otherwise.
 */
bool checkPasswordValidForUser(const std::string &userPasswordHash, const std::string &userLogin,
                               const ChatSystem &chatSystem) {

  auto user = findUserbyLogin(userLogin, chatSystem);

  bool check;
  auto passHash = user->getPassword();
  check = userPasswordHash == user->getPassword();

  return userPasswordHash == user->getPassword();
}

/**
 * @brief Prompts and validates a new login for user registration.
 * @param chatSystem Reference to the chat system for uniqueness checks.
 * @return A valid and unique login string, or "0" to cancel.
 */
std::string inputNewLogin(const ChatSystem &chatSystem) {
  while (true) {
    std::size_t dataLengthMin = 5;
    std::size_t dataLengthMax = 15;
    std::string prompt;

    prompt = "Введите новый Логин либо 0 для выхода. Не менее " + std::to_string(dataLengthMin) +
             " символов и не более " + std::to_string(dataLengthMax) + " символов (только латинские буквы и цифры) - ";

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
 * @brief Prompts and validates a new password for user registration.
 * @param chatSystem Reference to the chat system.
 * @return A hashed valid password string, or empty if canceled.
 */
std::string inputNewPassword(const ChatSystem &chatSystem) {

  std::size_t dataLengthMin = 5;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите новый Пароль либо 0 для выхода. Не менее " + std::to_string(dataLengthMin) +
           " символов и не более " + std::to_string(dataLengthMax) +
           ", обязательно одна заглавная буква и одна цифра (Только латинские буквы) - ";

  std::string newPassword = inputDataValidation(prompt, dataLengthMin, dataLengthMax, true, true, chatSystem);

  if (newPassword == "0")
    newPassword.clear();
  //   else
  //     newPassword = picosha2::hash256_hex_string(newPassword);

  return newPassword;
}

/**
 * @brief Prompts and validates a new display name for user registration.
 * @param chatSystem Reference to the chat system.
 * @return A valid name string, or empty if canceled.
 */
std::string inputNewName(const ChatSystem &chatSystem) {
  std::size_t dataLengthMin = 3;
  std::size_t dataLengthMax = 10;
  std::string prompt;

  prompt = "Введите желаемое Имя для отображениялибо 0 для выхода. Не менее " + std::to_string(dataLengthMin) +
           "символов и не более " + std::to_string(dataLengthMax) + ", (только латинские буквы и цифры) - ";

  std::string newName = inputDataValidation(prompt, dataLengthMin, dataLengthMax, false, true, chatSystem);

  if (newName == "0")
    newName.clear();

  return newName;
}

/**
 * @brief Handles the registration process for a new user.
 * @param chatSystem Reference to the chat system.
 * @details Collects and validates login, password, and display name, then adds
 * the new user to the system.
 */
void userRegistration(ChatSystem &chatSystem) {
  std::cout << "Регистрация нового пользователя." << std::endl;
  UserData userData;

  std::string newLogin = inputNewLogin(chatSystem);
  if (newLogin.empty() || newLogin == "0")
    return;

  std::string newPassword = inputNewPassword(chatSystem);
  if (newPassword.empty() || newPassword == "0")
    return;

  std::string newName = inputNewName(chatSystem);
  if (newName.empty())
    return;

  const auto &userPasswordHash = picosha2::hash256_hex_string(newPassword);

  auto newUser = std::make_shared<User>(UserData(newLogin, newName, userPasswordHash, "...@gmail.com", "+111"));

  chatSystem.addUser(newUser);
  newUser->createChatList(std::make_shared<UserChatList>(newUser));

  const auto &user = findUserbyLogin(newLogin, chatSystem);

  if (user == nullptr)
    std::cout << "user = nullptr";
  else {
    user->showUserData();

    // для проверки
    // std::cout << "DEBUG NewUser:" << std::endl;
    // std::cout << "Login: " << user->getLogin() << std::endl;
    // std::cout << "Name: " << user->getUserName() << std::endl;
    // std::cout << "PassHash: " << user->getPassword() << std::endl;
    // std::cout << "Email: " << user->getEmail() << std::endl;
    // std::cout << "Phone: " << user->getPhone() << std::endl;
  }
}

/**
 * @brief Handles user login procedure in the chat system.
 * @param chatSystem Reference to the chat system.
 * @return True if login is successful; false if canceled or failed.
 * @throws UserNotFoundException If the login does not exist.
 * @throws IncorrectPasswordException If the password is incorrect.
 */
bool userLoginInsystem(ChatSystem &chatSystem) {
  UserData userDataForLogin;
  std::string userLogin = "";
  std::string userPassword = "";

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

  while (true) {
    try {
      userPassword = inputDataValidation("Введите пароль (или 0 для выхода):", 0, 0, true, false, chatSystem);
      if (userPassword == "0")
        return false;

      const auto &userPasswordHash = picosha2::hash256_hex_string(userPassword);

      if (!checkPasswordValidForUser(userPasswordHash, userLogin, chatSystem))
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
