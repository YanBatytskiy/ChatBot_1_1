#include "menu/2_0_login_menu.h"
#include "exception/validation_exception.h"
#include "menu/2_1_new_chat_menu.h"
#include "menu/2_2_chat_list_menu.h"
#include "menu/2_4_user_profile.h"
#include "system/chat_system.h"
#include "system/system_function.h"
#include <cctype>
#include <iostream>
#include <ostream>

/**
 * @brief Displays the main authorization menu and retrieves user choice.
 * @return short Value corresponding to the user's choice:
 * - 1 — User registration
 * - 2 — Login to chat
 * - 0 — Exit program
 * @throws EmptyInputException If input is empty.
 * @throws IndexOutOfRangeException If input is not 0, 1, or 2.
 * @details Outputs a text menu to the console, processes user input, and returns the action code.
 */
short authMenu() { // вывод главного меню
  while (true) {
    std::cout << std::endl << "ChatBot 'Shark' Версия 1.0. @2025 \n \n";

    std::cout << "Выберите пункт меню: " << ::std::endl
              << "1. Регистрация пользователя " << std::endl
              << "2. Войти в ЧатБот" << std::endl
              << "0. Завершить программу" << std::endl;

    std::string userChoice;
    int userChoiceNumber;

    while (true) {
      std::getline(std::cin, userChoice);
      try {
        if (userChoice.empty())
          throw EmptyInputException();

        if (userChoice == "0")
          return 0;

        userChoiceNumber = parseGetlineToInt(userChoice);

        if (userChoiceNumber != 1 && userChoiceNumber != 2)
          throw IndexOutOfRangeException(userChoice);

        return userChoiceNumber;
      } catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      }
    } // first while
  } // second while
}

/**
 * @brief Handles user choices in the menu after authorization.
 * @param chatSystem Reference to the chat system.
 * @throws EmptyInputException If input is empty.
 * @throws IndexOutOfRangeException If input is not 0, 1, 2, 3, or 4.
 * @details Displays the post-login menu, processes user input, and directs to appropriate actions (new chat, chat list,
 * or user profile).
 */
void loginMenuChoice(ChatSystem &chatSystem) { // вывод главного меню
  int userChoiceNumber;
  std::string userChoice;

  while (true) {
    std::cout << std::endl;
    std::cout << "Добрый день, пользователь " << chatSystem.getActiveUser()->getUserName() << std::endl;
    std::cout << std::endl;
    std::cout << "Выберите пункт меню: " << std::endl;
    std::cout << "1 - Создать новый чат" << std::endl;
    std::cout << "2 - Показать список чатов" << std::endl;
    std::cout << "3 - Показать список папок - Under constraction." << std::endl;
    std::cout << "4 - Показать Профиль пользователя" << std::endl;
    std::cout << "0 - Выйти в предыдущее меню" << std::endl;

    bool exit2 = true;
    while (exit2) {
      std::getline(std::cin, userChoice);
      try {
        if (userChoice.empty())
          throw EmptyInputException();

        if (userChoice == "0")
          return;

        userChoiceNumber = parseGetlineToInt(userChoice);

        if (userChoiceNumber < 0 || userChoiceNumber > 4)
          throw IndexOutOfRangeException(userChoice);

        switch (userChoiceNumber) {
        case 1:
          LoginMenu_1NewChat(chatSystem);
          exit2 = false;
          continue; // case 1 MainMenu
        case 2:
          loginMenu_2ChatList(chatSystem);
          exit2 = false;
          continue; // case 2 MainMenu
        case 3:
          std::cout << "Показать список папок - Under constraction." << std::endl;
          break; // case 3 MainMenu
        case 4:
          loginMenu_4UserProfile(chatSystem);
          exit2 = false;
          continue; // case 4 MainMenu
        default:
          break; // default MainMenu
        } // switch
      } // try
      catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      }
    } // second
  }
}
