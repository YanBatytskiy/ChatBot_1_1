#include "menu/0_init_system.h"
#include "chat/chat.h"
#include "exception/validation_exception.h"
#include "message/message_content_struct.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief Constructor for InitDataArray.
 * @param messageText The text content of the message.
 * @param timeStamp The timestamp of the message.
 * @param sender Shared pointer to the sender user.
 * @param _recipients Vector of shared pointers to recipient users.
 * @details Initializes the InitDataArray struct with message details.
 */
InitDataArray::InitDataArray(std::string messageText, std::string timeStamp, std::shared_ptr<User> sender,
                             std::vector<std::shared_ptr<User>> _recipients)
    : _messageText(messageText), _timeStamp(timeStamp), _sender(sender), _recipients(_recipients) {}

/**
 * @brief Initializes the chat system with test data.
 * @param _chatsystem Reference to the ChatSystem object to be initialized.
 * @details Creates users, adds them to the system, creates chat lists, and sets up two chats (one private, one group)
 * with sample messages.
 */
void systemInitTest(ChatSystem &_chatsystem) {
  // Создание пользователей
  auto Alex2104_ptr = std::make_shared<User>("alex1980", "Sasha", "User01");
  auto Elena1510_ptr = std::make_shared<User>("elena1980", "Elena", "User0");
  auto Serg0101_ptr = std::make_shared<User>("serg1980", "Sergei", "User0");
  auto Vit2504_ptr = std::make_shared<User>("vit1980", "Vitaliy", "User0");
  auto mar1980_ptr = std::make_shared<User>("mar1980", "Mariya", "User0");
  auto fed1980_ptr = std::make_shared<User>("fed1980", "Fedor", "User0");
  auto vera1980_ptr = std::make_shared<User>("vera1980", "Vera", "User0");
  auto yak1980_ptr = std::make_shared<User>("yak1980", "Yakov", "User0");

  Alex2104_ptr->showUserData();
  Elena1510_ptr->showUserData();
  Serg0101_ptr->showUserData();
  Vit2504_ptr->showUserData();
  mar1980_ptr->showUserData();
  fed1980_ptr->showUserData();
  vera1980_ptr->showUserData();
  yak1980_ptr->showUserData();

  // Добавление пользователей в систему
  _chatsystem.addUser(Alex2104_ptr);
  _chatsystem.addUser(Elena1510_ptr);
  _chatsystem.addUser(Serg0101_ptr);
  _chatsystem.addUser(Vit2504_ptr);
  _chatsystem.addUser(mar1980_ptr);
  _chatsystem.addUser(fed1980_ptr);
  _chatsystem.addUser(vera1980_ptr);
  _chatsystem.addUser(yak1980_ptr);

  // Создание списков чатов для пользователей
  auto Alex2104_ChatList_ptr = std::make_shared<UserChatList>(Alex2104_ptr);
  auto Elena1510_ChatList_ptr = std::make_shared<UserChatList>(Elena1510_ptr);
  auto Serg0101_ChatList_ptr = std::make_shared<UserChatList>(Serg0101_ptr);
  auto Vit2504_ChatList_ptr = std::make_shared<UserChatList>(Vit2504_ptr);
  auto mar1980_ChatList_ptr = std::make_shared<UserChatList>(mar1980_ptr);
  auto fed1980_ChatList_ptr = std::make_shared<UserChatList>(fed1980_ptr);
  auto vera1980_ChatList_ptr = std::make_shared<UserChatList>(vera1980_ptr);
  auto yak1980_ChatList_ptr = std::make_shared<UserChatList>(yak1980_ptr);

  Alex2104_ptr->createChatList(Alex2104_ChatList_ptr);
  Elena1510_ptr->createChatList(Elena1510_ChatList_ptr);
  Serg0101_ptr->createChatList(Serg0101_ChatList_ptr);
  Vit2504_ptr->createChatList(Vit2504_ChatList_ptr);
  mar1980_ptr->createChatList(mar1980_ChatList_ptr);
  fed1980_ptr->createChatList(fed1980_ChatList_ptr);
  vera1980_ptr->createChatList(vera1980_ChatList_ptr);
  yak1980_ptr->createChatList(yak1980_ChatList_ptr);

  // Создание первого чата: Sasha и Elena (один на один)
  std::vector<std::shared_ptr<User>> recipients;
  std::vector<std::weak_ptr<User>> participants;

  recipients.push_back(Alex2104_ptr);
  participants.push_back(Elena1510_ptr);
  participants.push_back(Alex2104_ptr);

  auto chat_ptr = std::make_shared<Chat>();
  chat_ptr->addParticipant(Elena1510_ptr);
  chat_ptr->addParticipant(Alex2104_ptr);

  for (const auto &chatUser : participants) {
    try {
      if (auto chatUser_ptr = chatUser.lock()) {

        // добавление чата в чат-лист
        auto chatList = chatUser_ptr->getUserChatList();
        if (chatList) {
          chatList->addChat(chat_ptr);
        } else
          throw UnknownException("У пользователя нет списка чатов! init_system.");
      } else
        throw UnknownException("weak_ptr пустой. init_system.");
    } catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << std::endl;
      return;
    }
  }

  _chatsystem.addChat(chat_ptr);

  InitDataArray Elena_Alex1("Привет", "01-04-2025,12:00:00", Elena1510_ptr, recipients);
  addMessageToChat(Elena_Alex1, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);

  InitDataArray Elena_Alex2("Хай! как делишки?", "01-04-2025,12:05:00", Alex2104_ptr, recipients);
  addMessageToChat(Elena_Alex2, chat_ptr);

  recipients.clear();
  recipients.push_back(Alex2104_ptr);

  InitDataArray Elena_Alex3("Хорошо, как насчет кофе?", "01-04-2025,12:07:00", Elena1510_ptr, recipients);
  addMessageToChat(Elena_Alex3, chat_ptr);

  changeLastReadIndexForSender(Elena1510_ptr, chat_ptr);

  // проверки
  //   chat_ptr->printChat(Elena1510_ptr);

  // Создание второго чата: Elena, Sasha и Сергей (групповой чат)
  chat_ptr.reset();
  recipients.clear();
  participants.clear();

  recipients.push_back(Alex2104_ptr);
  recipients.push_back(Serg0101_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);
  participants.push_back(Elena1510_ptr);
  participants.push_back(Alex2104_ptr);
  participants.push_back(Serg0101_ptr);
  participants.push_back(mar1980_ptr);
  participants.push_back(yak1980_ptr);

  chat_ptr = std::make_shared<Chat>();
  chat_ptr->addParticipant(Elena1510_ptr);
  chat_ptr->addParticipant(Alex2104_ptr);
  chat_ptr->addParticipant(Serg0101_ptr);
  chat_ptr->addParticipant(mar1980_ptr);
  chat_ptr->addParticipant(yak1980_ptr);

  for (const auto &chatUser : participants) {
    if (auto chatUser_ptr = chatUser.lock()) {
      auto chatList = chatUser_ptr->getUserChatList();
      if (chatList) {
        chatList->addChat(chat_ptr);

      } else {
        std::cout << "[Ошибка] У пользователя нет списка чатов!\n" << std::endl;
      }
    }
  }
  _chatsystem.addChat(chat_ptr);

  InitDataArray Elena_Alex_Serg1("Всем Привееет!?", "01-04-2025,13:00:00", Elena1510_ptr, recipients);
  addMessageToChat(Elena_Alex_Serg1, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Serg0101_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg2("И тебе не хворать!?", "01-04-2025,13:02:00", Alex2104_ptr, recipients);
  addMessageToChat(Elena_Alex_Serg2, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg3("Всем здрассьте.", "01-04-2025,13:10:15", Serg0101_ptr, recipients);
  addMessageToChat(Elena_Alex_Serg3, chat_ptr);

  recipients.clear();
  recipients.push_back(Serg0101_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg4("Куда идем?", "01-04-2025,13:12:09", Elena1510_ptr, recipients);
  addMessageToChat(Elena_Alex_Serg4, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg5("В кино!", "01-04-2025,13:33:00", Serg0101_ptr, recipients);
  addMessageToChat(Elena_Alex_Serg5, chat_ptr);

  chat_ptr->updateLastReadMessageIndex(Elena1510_ptr, chat_ptr->getMessages().size() - 1);

  // проверки
  //    changeLastReadIndexForSender(Elena1510_ptr, chat_ptr);
  //   chat_ptr->printChat(Elena1510_ptr);
}
