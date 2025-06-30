#pragma once
#include "client-server/chat_system.h"

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