#pragma once
#include "ChatBot/chat_system.h"
#include <cstddef>
#include <memory>
#include <string>

// TODO: complete refactoring to use unordered_map for login procedures
// and user lookup by login:
// unordered_map<std::string, std::shared_ptr<User>> in ChatSystem

/**
 * @brief Validates a login or password string against defined constraints.
 * @param inputData The input string to validate.
 * @param contentLengthMin The minimum allowed length.
 * @param contentLengthMax The maximum allowed length.
 * @param isPassword True if validating a password, false if validating a login.
 * @return True if the input passes all validation checks.
 * @throws std::invalid_argument If the input does not meet requirements.
 */
bool checkNewDataInputForLimits(const std::string &inputData, std::size_t contentLengthMin,
                                std::size_t contentLengthMax, bool isPassword);

/**
 * @brief Prompts the user for input and validates it according to specified rules.
 * @param prompt The message shown to the user.
 * @param dataLengthMin The minimum length allowed for the input.
 * @param dataLengthMax The maximum length allowed for the input.
 * @param isPassword True if the input is a password.
 * @param newUserData True if creating a new user.
 * @param chatSystem Reference to the chat system.
 * @return The validated input string.
 * @throws EmptyInputException If the input is empty.
 */
std::string inputDataValidation(const std::string &prompt, std::size_t dataLengthMin, std::size_t dataLengthMax,
                                bool isPassword, bool newUserData,
                                const ChatSystem &chatSystem);

/**
 * @brief Searches for a user in the chat system by login.
 * @param userLogin The login to search for.
 * @param chatSystem Reference to the chat system.
 * @return A shared pointer to the user if found, or nullptr otherwise.
 */
std::shared_ptr<User> findUserbyLogin(const std::string &userLogin, const ChatSystem &chatSystem);

/**
 * @brief Checks whether a user with the specified login exists.
 * @param login The login to check.
 * @param chatSystem Reference to the chat system.
 * @return A shared pointer to the user if found, or nullptr otherwise.
 */
const std::shared_ptr<User> checkLoginExists(const std::string &login, const ChatSystem &chatSystem);

/**
 * @brief Verifies whether the given password matches the stored password for the user.
 * @param userPassword The password to verify.
 * @param userLogin The login of the user.
 * @param chatSystem Reference to the chat system.
 * @return True if the password is correct, false otherwise.
 */
bool checkPasswordValidForUser(const std::string &userPassword, const std::string& userLogin, const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new user login.
 * @param chatSystem Reference to the chat system for uniqueness checks.
 * @return A valid and unique login string, or "0" to cancel.
 */
std::string inputNewLogin(const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new password for user registration.
 * @param chatSystem Reference to the chat system.
 * @return A valid (hashed) password string, or empty if canceled.
 */
std::string inputNewPassword(const ChatSystem &chatSystem);

/**
 * @brief Prompts and validates a new display name for user registration.
 * @param chatSystem Reference to the chat system.
 * @return A valid display name, or empty if canceled.
 */
std::string inputNewName(const ChatSystem &chatSystem);

/**
 * @brief Handles the registration process for a new user.
 * @param chatSystem Reference to the chat system.
 * @details Collects and validates login, password, and display name, then adds the user.
 */
void userRegistration(ChatSystem &chatSystem);

/**
 * @brief Handles user login procedure in the chat system.
 * @param chatSystem Reference to the chat system.
 * @return True if login is successful, false if canceled or failed.
 */
bool userLoginInsystem(ChatSystem &chatSystem);
