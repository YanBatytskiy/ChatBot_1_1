#pragma once
#include "system/chat_system.h"

/**
 * @brief Displays the main authorization menu.
 * @return The user's menu selection as a short integer.
 */
short authMenu();

/**
 * @brief Handles the login menu flow and user input.
 * @param chatSystem Reference to the chat system.
 */
void loginMenuChoice(ChatSystem &chatSystem);

// /**
//  * @brief Opens the interface for the specified chat from the chat list.
//  * @param chatSystem Reference to the chat system.
//  * @param chat Shared pointer to the chat to open.
//  * @param Index Index of the chat in the list.
//  */
// void menuListOpenChat1(ChatSystem &chatSystem, const std::shared_ptr<Chat> &chat, std::size_t Index);

// /**
//  * @brief Opens the interface for searching and selecting a chat.
//  * @param chatSystem Reference to the chat system.
//  */
// void menuListFindChat2(ChatSystem &chatSystem);
