#pragma once
#include "client-server/chat_system.h"
#include <cstddef>
#include <memory>
#include <string>

// доделвть - добавить везде unordered map для процедур логина и поиска юзера по
// логину
// unordered_map<std::string, std::shared_ptr<User>> в ChatSystem

/**
 * @brief Validates login or password input against specified constraints.
 * @param inputData The input string to validate.
 * @param contentLengthMin Minimum allowed length.
 * @param contentLengthMax Maximum allowed length.
 * @param isPassword True if validating a password, false for login.
 * @return true if the input is valid.
 * @throws std::invalid_argument If the input does not meet requirements.
 */
bool checkNewDataInputForLimits(const std::string &inputData, std::size_t contentLengthMin,
                                std::size_t contentLengthMax, bool isPassword);

/**
 * @brief Validates user input with retry attempts.
 * @param prompt Message displayed to the user.
 * @param dataLengthMin Minimum length of the input.
 * @param dataLengthMax Maximum length of the input.
 * @param isPassword True if input is a password.
 * @param dataForValidation Structure for storing/comparing input data.
 * @param newUserData True if creating a new user.
 * @param chatSystem Reference to the chat system.
 * @return Validated input string.
 * @details Displays a prompt, retrieves user input, validates it, and returns the validated value.
 */
std::string inputDataValidation(const std::string &prompt, std::size_t dataLengthMin, std::size_t dataLengthMax,
                                bool isPassword, bool newUserData,
                                const ChatSystem &chatSystem);

/**
 * @brief Finds a user by login in the chat system.
 * @param userLogin The login to search for.
 * @param chatSystem Reference to the chat system.
 * @return Shared pointer to the found user, or nullptr if not found.
 */
std::shared_ptr<User> findUserbyLogin(const std::string &userLogin, const ChatSystem &chatSystem);

/**
 * @brief Checks if a user with the specified login exists.
 * @param login The login to check.
 * @param chatSystem Reference to the chat system.
 * @return Shared pointer to the user if found, or nullptr if not found.
 */
const std::shared_ptr<User> checkLoginExists(const std::string &login, const ChatSystem &chatSystem);

/**
 * @brief Verifies if the provided password matches the user's password.
 * @param userData Structure containing login and password.
 * @param chatSystem Reference to the chat system.
 * @return True if the password is valid, false otherwise.
 */
bool checkPasswordValidForUser(const std::string &userPassword, const std::string& userLogin, const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new user login.
 * @param chatSystem Reference to the chat system for uniqueness checks.
 */
std::string inputNewLogin(const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new user password.
 * @param chatSystem Reference to the chat system.
 */
std::string inputNewPassword(const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new user display name.
 * @param chatSystem Reference to the chat system.
 */
std::string inputNewName(const ChatSystem &chatSystem);

/**
 * @brief Performs the full registration process for a new user.
 * @param chatSystem Reference to the chat system.
 * @details Handles input and validation of login, password, and name.
 */
void userRegistration(ChatSystem &chatSystem);

/**
 * @brief Authenticates a user in the chat system.
 * @param chatSystem Reference to the chat system.
 * @return True if login is successful, false if login fails.
 */
bool userLoginInsystem(ChatSystem &chatSystem);

