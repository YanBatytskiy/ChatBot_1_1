#include "menu/2_1_new_chat_menu.h"
#include "chat/chat.h"
#include "exception/login_exception.h"
#include "exception/validation_exception.h"
#include "system/system_function.h"
#include "user/user_chat_list.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Creates a new chat by selecting participants.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat being created.
 * @param activeUserIndex Index of the active user.
 * @param target Target type for the message (e.g., individual, several, or all).
 * @throws EmptyInputException If input is empty.
 * @throws InvalidCharacterException If input contains invalid characters.
 * @throws UserNotFoundException If no users are found for the search query.
 * @throws IndexOutOfRangeException If selected index is out of range.
 * @details Handles user selection for adding participants to a new chat based on the target type.
 */
void LoginMenu_1NewChatMakeParticipants(ChatSystem &chatSystem, std::shared_ptr<Chat> &chat,
                                        std::size_t activeUserIndex,
                                        MessageTarget target) { // создение нового сообщения путем выбора пользователей

  std::string inputData;
  std::string userChoice;
  int userChoiceNumber;

  // добавляем отправителя в вектор участников
  chat->addParticipant(chatSystem.getActiveUser());

  switch (target) {

  case MessageTarget::One: {
    // сообщение только одному пользователю

    bool exit = true;
    while (exit) {
      try {
        // получаем список получателей через ввод списка
        std::cout << "Введите часть имени или логина для поиска большими или маленькими буквами или 0 для отмены: "
                  << std::endl;

        getline(std::cin, inputData);

        if (inputData.empty())
          throw EmptyInputException();

        if (inputData == "0") {
          exit = false;
          continue;
        }

        // проверяем на наличие недопустимых символов
        for (const auto &exactChar : inputData)
          if (!std::isdigit(exactChar) && !std::isalpha(exactChar))
            throw InvalidCharacterException(exactChar);

        // найти пользователей
        std::vector<std::shared_ptr<User>> users;
        chatSystem.findUser(users, inputData);

        if (users.size() == 0)
          throw UserNotFoundException();

        // выводим найденных пользователей на экран
        int index = 1;
        std::cout << "Вот, кого мы нашли (Имя : Логин), выберите одного:" << std::endl;
        for (const auto &user : users) {
          std::cout << index << ". " << user->getUserName() << " : " << user->getLogin() << std::endl;
          ++index;
        }

        // выбрать нужного из списка
        bool exit2 = true;
        while (exit2) {
          try {
            // выбираем пользователя
            std::cout << "Введите номер пользователя 0 для отмены: " << std::endl;

            getline(std::cin, inputData);

            if (inputData.empty())
              throw EmptyInputException();

            if (inputData == "0") {
              exit = false;
              break;
            }

            userChoiceNumber = parseGetlineToInt(inputData);

            if (userChoiceNumber > index)
              throw IndexOutOfRangeException(inputData);

            break;
          } // try
          catch (const ValidationException &ex) {
            std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
            continue;
          }
          //   exit2 = false;
        } // second while exit2

        // здесь мы проверяем, ято если пользователь ввел ноль, то поиск нужно заново начать
        if (exit == false) {
          exit = true;
          continue;
        }

        // заполнить вектор участников
        chat->addParticipant(users[userChoiceNumber - 1]);

        // проверки
        std::cout << "Участники чата: " << std::endl;
        for (const auto &user : chat->getParticipants()) {
          auto user_ptr = user._user.lock();
          std::cout << user_ptr->getLogin() << " ака " << user_ptr->getUserName() << std::endl;
        }

      } // try
      catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      }
      exit = false;
    } // first while exit

    break;
  } // case One

  case MessageTarget::Several: {

    // сообщение только некоторым пользователям

    bool exit = true;
    while (exit) {
      try {
        // получаем список получателей через ввод списка
        std::cout << "Введите номера пользователей через запятую или 0 для отмены: " << std::endl;

        getline(std::cin, inputData);

        if (inputData.empty())
          throw EmptyInputException();

        if (inputData == "0") {
          exit = false;
          continue;
        }

        // проверяем на наличие недопустимых символов
        for (const auto &exactChar : inputData)
          if (!std::isdigit(exactChar) && static_cast<unsigned char>(exactChar) != ',')
            throw InvalidCharacterException(exactChar);

        // проверки
        // std::cout << activeUserIndex << std::endl;

        // добавляем в конец запятую
        inputData.push_back(',');
        // проверки
        // std::cout << std::endl << inputData << std::endl;

        // создаем вектор для хранения индексов получателей в массиве пользователей
        std::vector<std::size_t> recipientIndex;

        // заполняем вектор значениями
        std::string str_temp;
        std::size_t index_temp;
        std::size_t userCount = chatSystem.getUsers().size();

        do {
          auto pos = inputData.find_first_of(',');

          str_temp = inputData.substr(0, pos);
          inputData.erase(0, pos + 1);
          // std::cout << str_temp << " : ";

          if (!str_temp.empty()) {
            index_temp = parseGetlineToSizeT(str_temp);
            // std::cout << index_temp << std::endl;

            if (index_temp == 0 || index_temp > userCount - 1)
              throw IndexOutOfRangeException(str_temp);

            // Преобразуем ввод в индекс относительно getUsers
            std::size_t realIndex = (index_temp <= activeUserIndex) ? index_temp - 1 : index_temp;

            if (realIndex >= userCount)
              throw IndexOutOfRangeException("Индекс выходит за пределы getUsers()");

            // проверяем на дубли
            if (std::find(recipientIndex.begin(), recipientIndex.end(), realIndex) == recipientIndex.end()) {
              recipientIndex.push_back(realIndex);
            }
          };

        } while (!inputData.empty());

        // проверки временная
        std::cout << std::endl;
        for (const auto &rep : recipientIndex)
          std::cout << rep << ", ";

        // проверки временная
        std::cout << std::endl;
        for (std::size_t i = 1; i <= recipientIndex.size(); ++i) {
          std::cout << chatSystem.getUsers()[recipientIndex[i - 1]]->getLogin() << ", ";
        }

        // заполняем вектор участников
        for (const auto &recipient : recipientIndex) {
          chat->addParticipant(chatSystem.getUsers()[recipient]);
        }

        // проверки

        std::cout << "Участники чата: " << std::endl;
        for (const auto &user : chat->getParticipants()) {
          auto user_ptr = user._user.lock();
          std::cout << user_ptr->getLogin() << " ака " << user_ptr->getUserName() << std::endl;
        }
      } // try
      catch (const ValidationException &ex) {
        std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
        continue;
      }
      exit = false;
    } // while
    break;
  } // case Severall
  case MessageTarget::All: {
    // заполняем вектор участников чата
    for (const auto &user : chatSystem.getUsers())
      if (user != chatSystem.getActiveUser()) {
        chat->addParticipant(user);
      }

    return; // case All
  }

  } // switch
}

/**
 * @brief Creates and sends a message to a new chat.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the new chat.
 * @param activeUserIndex Index of the active user.
 * @param target Target type for the message (e.g., individual, several, or all).
 * @throws BadWeakException If a weak_ptr cannot be locked.
 * @throws ValidationException If message input fails validation.
 * @details Manages participant selection, message input, and chat integration into the system.
 */
void CreateAndSendNewChat(ChatSystem &chatSystem, std::shared_ptr<Chat> &chat, std::size_t activeUserIndex,
                          MessageTarget target) {

  LoginMenu_1NewChatMakeParticipants(chatSystem, chat, activeUserIndex, target);

  // создаем сообщение
  std::cout << std::endl << "Вот твой чат. В нем всего 0 сообщения(ий). " << std::endl;

  bool exitCase2 = true;
  std::size_t unReadCount = 0;

  while (exitCase2) {
    try {
      if (inputNewMessage(chatSystem, chat) == false) { // пользователь решмил не вводить сообщение
        exitCase2 = false;
      } else { // пользователь ввел сообщение
        if (unReadCount == 0) {
          // добавили в головную систему чтобы не потерять чат при выходе из метода
          chatSystem.addChat(chat);

          // добавили каждому участнику чат в чат-лист
          for (const auto &user : chat->getParticipants()) {
            auto user_ptr = user._user.lock();
            if (user_ptr) {
              user_ptr->getUserChatList()->addChat(chat);
            } else
              throw BadWeakException("LoginMenu_1NewChat");
          }
        }
        ++unReadCount;

        // проверки
        // chat->printChat(chatSystem.getActiveUser());
        // std::cout << std::endl;
      }; // else
    } // try
    catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << std::endl;
    }
  } // while case 2
}

/**
 * @brief Initiates the creation of a new chat.
 * @param chatSystem Reference to the chat system.
 * @throws EmptyInputException If input is empty.
 * @throws IndexOutOfRangeException If input is not 0, 1, 2, or 3.
 * @details Provides a menu for selecting the type of new chat (one user, several users, or all users).
 */
void LoginMenu_1NewChat(ChatSystem &chatSystem) { // создание нового сообщения

  std::string userChoice;
  size_t userChoiceNumber;
  bool exit = true;
  // создали новый чат
  auto chat = std::make_shared<Chat>();

  while (exit) {
    std::cout << "Хотите: " << std::endl
              << "1. Найти пользователя и отправить ему сообщение" << std::endl
              << "2. Вывести список пользователей и отправить нескольким пользователям" << std::endl
              << "3. Отправить сообщение всем пользователям" << std::endl
              << "0. Для выхода в предыдущее меню" << std::endl;

    std::getline(std::cin, userChoice);

    try {

      if (userChoice.empty())
        throw EmptyInputException();

      if (userChoice == "0")
        return;

      userChoiceNumber = parseGetlineToInt(userChoice);

      if (userChoiceNumber != 1 && userChoiceNumber != 2 && userChoiceNumber != 3)
        throw IndexOutOfRangeException(userChoice);

      switch (userChoiceNumber) {

      case 1: { // 1. Найти пользователя и отправить ему сообщение
        CreateAndSendNewChat(chatSystem, chat, 0, MessageTarget::One);

        exit = false; // выход в верхнее меню так как новый чат уже не новый
        break;        // case 1
      }
      case 2: { // 2. Вывести список пользователей и отправить нескольким пользователям

        // запомнили номер активного пользователя
        auto activeUserIndex = chatSystem.showUserList(false);

        CreateAndSendNewChat(chatSystem, chat, activeUserIndex, MessageTarget::Several);

        exit = false; // выход в верхнее меню так как новый чат уже не новый
        break;        // case 2
      }
      case 3: { // 3. Отправить сообщение всем пользователям

        CreateAndSendNewChat(chatSystem, chat, 0, MessageTarget::All);
        exit = false; // выход в верхнее меню так как новый чат уже не новый
        break;        // case 3
      }
      default:
        break; // default
      } // switch
    } // try
    catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
  } // while
}
