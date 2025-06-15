#pragma once
#include "system/chat_system.h"
#include "system/system_function.h"

/**
 * @brief Creates a new chat by selecting participants.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat being created.
 * @param activeUserIndex Index of the active user.
 * @param target Target type for the message (e.g., individual or group).
 * @details Facilitates the selection of users to add as participants to a new chat.
 */
void LoginMenu_1NewChatMakeParticipants(ChatSystem &chatSystem, std::shared_ptr<Chat> &chat,
                                        std::size_t activeUserIndex,
                                        MessageTarget target); // создение нового сообщения путем выбора пользователей

/**
 * @brief Creates and sends a message to a new chat.
 * @param chatSystem Reference to the chat system.
 * @param activeUserIndex Index of the active user.
 * @param sendToAll True if the message should be sent to all users.
 * @details Handles the creation of a new chat and sending a message, supporting different sending modes.
 */
void CreateAndSendNewChat(ChatSystem &chatSystem, std::size_t activeUserIndex,
                          bool sendToAll); // общая функция для отправки сообщения в новый чат тремя способами

/**
 * @brief Initiates the creation of a new chat.
 * @param chatSystem Reference to the chat system.
 * @details Provides the interface for starting the process of creating a new chat.
 */
void LoginMenu_1NewChat(ChatSystem &chatSystem); // создание нового сообщения
