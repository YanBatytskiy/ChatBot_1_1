#include "0_init_system.h"
#include "chat/chat.h"
#include "chat_system.h"
#include "exception/validation_exception.h"
#include "message/message_content_struct.h"
#include "system/picosha2.h"
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
 * @param messageId Unique message ID.
 */

InitDataArray::InitDataArray(std::string messageText, std::string timeStamp, std::shared_ptr<User> sender,
                             std::vector<std::shared_ptr<User>> _recipients, std::size_t messageId)
    : _messageText(messageText), _timeStamp(timeStamp), _sender(sender), _recipients(_recipients),
      _messageId(messageId) {}

/**
 * @brief Initializes the chat system with test data.
 * @param _chatsystem Reference to the ChatSystem object to be initialized.
 * @details Creates users, adds them to the system, creates chat lists, and sets up two chats (one private, one group)
 * with sample messages.
 */

// const std::string initUserPassword[] = {"User01", "User02", "User03", "User04", "User05",
//                                         "User06", "User07", "User08"};
const std::string initUserPassword[] = {"1", "1", "1", "1", "1", "1", "1", "1"};

// const std::string initUserLogin[] = {"alex1980", "elena1980", "serg1980", "vit1980",
//                                      "mar1980",  "fed1980",   "vera1980", "yak1980"};

const std::string initUserLogin[] = {"a", "e", "s", "v", "m", "f", "ver", "y"};

void systemInitTest(ChatSystem &_chatsystem) {
  // Создание пользователей
  std::string passwordHash = picosha2::hash256_hex_string(initUserPassword[0]);
  auto Alex2104_ptr = std::make_shared<User>(
      UserData(initUserLogin[0], "Sasha", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[1]);
  auto Elena1510_ptr = std::make_shared<User>(
      UserData(initUserLogin[1], "Elena", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[2]);
  auto Serg0101_ptr = std::make_shared<User>(
      UserData(initUserLogin[2], "Sergei", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[3]);
  auto Vit2504_ptr = std::make_shared<User>(
      UserData(initUserLogin[3], "Vitaliy", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[4]);
  auto mar1980_ptr = std::make_shared<User>(
      UserData(initUserLogin[4], "Mariya", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[5]);
  auto fed1980_ptr = std::make_shared<User>(UserData(initUserLogin[5], "Fedor", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[6]);
  auto vera1980_ptr = std::make_shared<User>(UserData(initUserLogin[6], "Vera", passwordHash, "...@gmail.com", "+111"));
  passwordHash = picosha2::hash256_hex_string(initUserPassword[7]);
  auto yak1980_ptr = std::make_shared<User>(UserData(initUserLogin[7], "Yakov", passwordHash, "...@gmail.com", "+111"));

  Alex2104_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[0] << std::endl;
  Elena1510_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[1] << std::endl;
  Serg0101_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[2] << std::endl;
  Vit2504_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[3] << std::endl;
  mar1980_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[4] << std::endl;
  fed1980_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[5] << std::endl;
  vera1980_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[6] << std::endl;
  yak1980_ptr->showUserDataInit();
  std::cout << ", Password: " << initUserPassword[7] << std::endl;

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

  InitDataArray Elena_Alex1("Привет", "01-04-2025,12:00:00", Elena1510_ptr, recipients, _chatsystem.getNewMessageId());

  addMessageToChat(Elena_Alex1, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);

  InitDataArray Elena_Alex2("Хай! как делишки?", "01-04-2025,12:05:00", Alex2104_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex2, chat_ptr);

  recipients.clear();
  recipients.push_back(Alex2104_ptr);

  InitDataArray Elena_Alex3("Хорошо, как насчет кофе?", "01-04-2025,12:07:00", Elena1510_ptr, recipients, _chatsystem.getNewMessageId());
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

//   chat_ptr->addChatId(_chatsystem.getNewChatId());

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

  InitDataArray Elena_Alex_Serg1("Всем Привееет!?", "01-04-2025,13:00:00", Elena1510_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex_Serg1, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Serg0101_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg2("И тебе не хворать!?", "01-04-2025,13:02:00", Alex2104_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex_Serg2, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg3("Всем здрассьте.", "01-04-2025,13:10:15", Serg0101_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex_Serg3, chat_ptr);

  recipients.clear();
  recipients.push_back(Serg0101_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg4("Куда идем?", "01-04-2025,13:12:09", Elena1510_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex_Serg4, chat_ptr);

  recipients.clear();
  recipients.push_back(Elena1510_ptr);
  recipients.push_back(Alex2104_ptr);
  recipients.push_back(mar1980_ptr);
  recipients.push_back(yak1980_ptr);

  InitDataArray Elena_Alex_Serg5("В кино!", "01-04-2025,13:33:00", Serg0101_ptr, recipients, _chatsystem.getNewMessageId());
  addMessageToChat(Elena_Alex_Serg5, chat_ptr);

  chat_ptr->updateLastReadMessageIndex(Elena1510_ptr, chat_ptr->getMessages().size() - 1);

  // проверки
  //    changeLastReadIndexForSender(Elena1510_ptr, chat_ptr);
  //   chat_ptr->printChat(Elena1510_ptr);
}