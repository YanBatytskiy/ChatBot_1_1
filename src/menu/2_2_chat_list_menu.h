#pragma once
#include "system/chat_system.h"

/**
 * @brief Displays the list of chats for the active user.
 * @param chatSystem Reference to the chat system.
 * @details Shows available chats and allows the user to interact with them.
 */
void loginMenu_2ChatList(ChatSystem &chatSystem);

/**
 * @brief Edits or manages a specific chat.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat to be edited.
 * @details Provides options to modify or interact with the specified chat.
 */
void loginMenu_2EditChat(ChatSystem &chatSystem, const std::shared_ptr<Chat> &chat /*, std::size_t unReadCountIndex*/);