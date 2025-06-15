#pragma once
#include "message/message_content_struct.h"
#include "system/chat_system.h"
#include <alloca.h>
#include <string>

/**
 * @brief Enum class defining message target types.
 */
enum class MessageTarget {One, Several, All};

/**
 * @brief Converts a string to an integer.
 * @param str The string to convert.
 * @return The converted integer value.
 */
int parseGetlineToInt(const std::string &str); // конвертация из string в int

/**
 * @brief Converts a string to a size_t.
 * @param str The string to convert.
 * @return The converted size_t value.
 */
std::size_t parseGetlineToSizeT(const std::string &str); // конвертация из string в size_t

/**
 * @brief Updates the last read message index for the sender in a chat.
 * @param user Shared pointer to the user (sender).
 * @param chat Shared pointer to the chat.
 */
void changeLastReadIndexForSender(const std::shared_ptr<User> &user, const std::shared_ptr<Chat> &chat);

/**
 * @brief Adds a message to a chat using initial data.
 * @param initDataArray Structure containing message data.
 * @param chat Shared pointer to the chat.
 */
void addMessageToChat(const InitDataArray &initDataArray, std::shared_ptr<Chat> &chat);

/**
 * @brief Prompts the user to input a new message for a chat.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat.
 * @return True if a message was successfully added, false otherwise.
 */
bool inputNewMessage(ChatSystem &chatSystem, std::shared_ptr<Chat> chat);

/**
 * @brief Converts a string to lowercase.
 * @param str The input string.
 * @return The lowercase version of the input string.
 */
std::string TextToLower(const std::string &str); // приведение к нижнему регистру

