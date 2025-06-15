#include "menu/0_init_system.h"
#include "menu/1_registration.h"
#include "menu/2_0_login_menu.h"
#include "system/chat_system.h"
#include <iostream>

/**
 * @brief Main entry point for the chat system application.
 * @return 0 on successful execution or exit.
 * @details Initializes the chat system, handles user authentication (registration/login), and manages the main program
 * loop.
 */
int main() {
  // Create ChatSystem instance
  ChatSystem chatSystem;

  // Initialize the system with test data
  systemInitTest(chatSystem);

  short userChoice;

  // Main program loop
  while (true) {
    // Reset active user
    chatSystem.setActiveUser(nullptr);

    // Set a default active user with login "E"
    // std::shared_ptr<User> activeUser_ptr = findUserbyLogin("E", chatSystem);
    // chatSystem.setActiveUser(activeUser_ptr);

    // Display authentication menu and get user choice
    userChoice = authMenu();

    // Handle user choice for registration, login, or exit
    switch (userChoice) {
    case 0: // Exit the program
      return 0;
    case 1: // Register a new user
      userRegistration(chatSystem);
      break;
    case 2: // Log in an existing user
      if (userLoginInsystem(chatSystem))
        loginMenuChoice(chatSystem);
      break;
    default:
      break; // Handle invalid choices
    }
  }

  std::cout << std::endl;
  return 0;
}