#include "exception/validation_exception.h"
#include "system/chat_system.h"
#include "system/system_function.h"
#include "user/user_chat_list.h"
#include <iostream>

/**
 * @brief Manages interactions with a specific chat.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat to be edited.
 * @throws EmptyInputException If input is empty.
 * @throws IndexOutOfRangeException If input is not 0, 1, 2, 3, 4, or 5.
 * @details Displays chat details, participants, and messages, and provides options to send messages or perform other
 * actions (some under construction).
 */
void loginMenu_2EditChat(ChatSystem &chatSystem, const std::shared_ptr<Chat> &chat /*, std::size_t unReadCountIndex*/) {

  std::string userChoice;
  size_t userChoiceNumber;
  bool exit = true;

  while (exit) {
    auto messageCount = chat->getMessages().size();

    auto unReadCount = chat->getLastReadMessageIndex(chatSystem.getActiveUser()); // получаем количество неппрочитанных

    std::cout << std::endl << "Вот твой чат. В нем всего " << messageCount << " сообщения(ий). ";
    std::cout << "\033[32m"; // red
    std::cout << "Из них непрочитанных - " << messageCount - unReadCount << std::endl;
    std::cout << "\033[0m";

    // выводим список участников чата кроме активного юзера
    std::cout << std::endl << "Участники чата Имя/Логин: " << std::endl;
    // перебираем участников чата
    for (const auto &participant : chat->getParticipants()) {
      auto user_ptr = participant._user.lock();
      if (user_ptr) {
        if (user_ptr != chatSystem.getActiveUser()) {
          std::cout << user_ptr->getUserName() << "/" << user_ptr->getUserName() << "; ";
        };
      } else {
        std::cout << "удал. пользоыватель";
      }
    }

    std::cout << std::endl;

    chat->printChat(chatSystem.getActiveUser());
    chat->updateLastReadMessageIndex(chatSystem.getActiveUser(), messageCount);
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << "Что будем делать? " << std::endl;
    std::cout << "1 - написать сообщение" << std::endl;
    std::cout << "2 - удалить последние отправленные сообщения - Under constraction" << std::endl;
    std::cout << "3 - очистить чат - Under constraction" << std::endl;
    std::cout << "4 - выйти из чата/удалить чат у пользователя - Under constraction" << std::endl;
    std::cout << "5 - поиск внутри чата - Under constraction" << std::endl;
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

        if (userChoiceNumber < 1 || userChoiceNumber > 5)
          throw IndexOutOfRangeException(userChoice);

        switch (userChoiceNumber) {
        case 1:
          inputNewMessage(chatSystem, chat);
          chat->updateLastReadMessageIndex(chatSystem.getActiveUser(), messageCount + 1);
          std::cout << std::endl;

          chat->printChat(chatSystem.getActiveUser());
          std::cout << std::endl;

          exit2 = false;
          break; // case 1
        case 2:
          std::cout << "2 - удалить последние отправленные сообщения - Under constraction" << std::endl;
          break; // case 2
        case 3:
          std::cout << "3 - очистить чат - Under constraction" << std::endl;
          break; // case 3
        case 4:
          std::cout << "4 - выйти из чата/удалить чат у пользователя - Under constraction" << std::endl;
          break; // case 4
        case 5:
          std::cout << "5 - поиск внутри чата - Under constraction" << std::endl;
          break; // case 5
        default:
          break; // default
        } // switch
      } catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      }

    } // second while
  } // first while
}

/**
 * @brief Displays the list of chats for the active user.
 * @param chatSystem Reference to the chat system.
 * @throws EmptyInputException If input is empty.
 * @throws IndexOutOfRangeException If input is not a valid chat index or 0.
 * @throws ChatNotFoundException If the selected chat cannot be accessed.
 * @details Shows the user's chat list and allows selection of a chat to edit or view.
 */
void loginMenu_2ChatList(ChatSystem &chatSystem) { // показать список чатов

  auto chatCount = chatSystem.getActiveUser()
                       ->getUserChatList()
                       ->getChatFromList()
                       .size(); // количество чатов у пользователя

  std::cout << std::endl;

  chatSystem.getActiveUser()->printChatList(chatSystem.getActiveUser()); // определяем текущего пользователя
  std::cout << std::endl;

  if (chatSystem.getActiveUser()->getUserChatList()->getChatFromList().empty()) {
    std::cout << "У пользователя пока нет чатов" << std::endl;
    return;
  }

  std::string userChoice;
  int userChoiceNumber;
  bool exit2 = true;

  while (true) {

    std::cout << "Выберите пункт меню: " << std::endl;
    std::cout << "От 1 до " << chatCount << " - Чтобы открыть чат введите его номер (всего " << chatCount
              << " чата(ов)): " << std::endl;
    std::cout << "f - Поиск по чатам. Under constraction." << std::endl;
    std::cout << "0 - Выйти в предыдущее меню" << std::endl;

    while (exit2) {
      std::getline(std::cin, userChoice);

      try {

        if (userChoice.empty())
          throw EmptyInputException();

        if (userChoice == "0")
          return;

        if (userChoice == "f") {
          std::cout << "f - Поиск по чатам. Under constraction." << std::endl;
          exit2 = false;
          continue;
        }

        userChoiceNumber = parseGetlineToInt(userChoice);

        if (userChoiceNumber < 0 || userChoiceNumber > static_cast<int>(chatCount))
          throw IndexOutOfRangeException(userChoice);

        // здесь мы достаем из вектора количество непрочитанных сообщениотображения на экране
        auto chatList = chatSystem.getActiveUser()->getUserChatList()->getChatFromList(); // weak указатель на вектор

        auto activeChat_weak = chatList[userChoiceNumber - 1];
        auto activeChat_ptr = activeChat_weak.lock();

        if (!activeChat_ptr)
          throw ChatNotFoundException();
        else
          loginMenu_2EditChat(chatSystem, activeChat_ptr);
        return;
      } catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      };
    } // second while

  } // первый while
}
