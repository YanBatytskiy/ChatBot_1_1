#include "system/system_function.h"
#include "chat/chat.h"
#include "exception/validation_exception.h"
#include "menu/0_init_system.h"
#include "message/message_content_struct.h"
#include "system/chat_system.h"
#include "system/date_time_utils.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <limits>
#include <locale>
#include <stdexcept>
#include <string>

/**
 * @brief Получает индекс символа в алфавите.
 * @param word Строка, содержащая символ (может быть многобайтный UTF-8).
 * @return Индекс символа в массиве alphabet или -1, если не найден.
 */
int getCharIndex(const std::string &word) {
  if (word.empty())
    return -1;
  std::size_t charLen = getUtf8CharLen((unsigned char)word[0]);

  if (charLen > word.size())
    return -1;

  std::string ch = word.substr(0, charLen);

  for (int i = 0; i < ALPHABET_SIZE; ++i)
    if (ch == alphabet[i])
      return i;

  return -1;
}

/**
 * @brief Определяет длину UTF-8 символа по первому байту.
 * @param ch Первый байт символа.
 * @return Количество байт в символе (1-4).
 */
std::size_t getUtf8CharLen(unsigned char ch) {
  if ((ch & 0x80) == 0)
    return 1;
  else if ((ch & 0xE0) == 0xC0)
    return 2;
  else if ((ch & 0xF0) == 0xE0)
    return 3;
  else if ((ch & 0xF8) == 0xF0)
    return 4;
  return 1;
}

/**
 * @brief Включает поддержку UTF-8 в консоли.
 *
 * @details Настраивает глобальную локаль и кодировку консоли.
 * Работает стабильно на Windows (MSYS2/MinGW), Linux и macOS.
 */
void enableUTF8Console() {
#if defined(_WIN32)
  try {
    std::locale utf8_locale(""); // системная локаль (обычно UTF-8 в MSYS2)
    std::locale::global(utf8_locale);

    std::wcin.imbue(utf8_locale);
    std::wcout.imbue(utf8_locale);
    std::wcerr.imbue(utf8_locale);
  } catch (const std::exception &e) {
    std::cerr << "Failed to set UTF-8 locale on Windows: " << e.what()
              << std::endl;
  }

  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

#else
  try {
    std::locale utf8_locale("en_US.UTF-8");
    std::locale::global(utf8_locale);

    std::wcin.imbue(utf8_locale);
    std::wcout.imbue(utf8_locale);
    std::wcerr.imbue(utf8_locale);
  } catch (const std::exception &e) {
    std::cerr << "Failed to set UTF-8 locale on Unix: " << e.what()
              << std::endl;
  }
#endif
}

/**
 * @brief Converts a string to an integer.
 * @param str The string to convert.
 * @return The converted integer value.
 * @throws NonDigitalCharacterException If the string contains non-numeric
 * characters.
 * @throws IndexOutOfRangeException If the value exceeds the integer range.
 */
int parseGetlineToInt(const std::string &str) { // конвертация из string в int
  try {
    long long value = std::stoll(str);

    if (value < std::numeric_limits<int>::min() ||
        value > std::numeric_limits<int>::max()) {
      throw std::out_of_range("value exceeds int range");
    }
    return static_cast<int>(value);

  } catch (const std::invalid_argument &) {
    throw NonDigitalCharacterException();
  } catch (const std::out_of_range &) {
    throw IndexOutOfRangeException(str);
  }
}

/**
 * @brief Converts a string to a size_t.
 * @param str The string to convert.
 * @return The converted size_t value.
 * @throws NonDigitalCharacterException If the string contains non-numeric
 * characters.
 * @throws IndexOutOfRangeException If the value exceeds the size_t range.
 */
std::size_t
parseGetlineToSizeT(const std::string &str) { // конвертация из string в size_t
  try {
    unsigned long long value = std::stoull(str);

    if (value >
        static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
      throw std::out_of_range("value exceeds size_t range");
    }
    return static_cast<std::size_t>(value);

  } catch (const std::invalid_argument &) {
    throw NonDigitalCharacterException();
  } catch (const std::out_of_range &) {
    throw IndexOutOfRangeException(str);
  }
}

/**
 * @brief Updates the last read message index for the sender in a chat.
 * @param user Shared pointer to the user (sender).
 * @param chat Shared pointer to the chat.
 */
void changeLastReadIndexForSender(const std::shared_ptr<User> &user,
                                  const std::shared_ptr<Chat> &chat) {

  chat->updateLastReadMessageIndex(user, chat->getMessages().size());
}

/**
 * @brief Adds a message to a chat using initial data.
 * @param initDataArray Structure containing message data.
 * @param chat Shared pointer to the chat.
 * @throws UnknownException If the sender is null.
 */
void addMessageToChat(const InitDataArray &initDataArray,
                      std::shared_ptr<Chat> &chat) {

  std::vector<std::shared_ptr<IMessageContent>> iMessageContent;
  TextContent textContent(initDataArray._messageText);
  MessageContent<TextContent> messageContentText(textContent);
  iMessageContent.push_back(
      std::make_shared<MessageContent<TextContent>>(messageContentText));

  try {
    if (!initDataArray._sender) {
      throw UnknownException(" Отправитель отсутствует. Сообщение не будет "
                             "создано. addMessageToChat");
    } else {
      Message message(iMessageContent, initDataArray._sender,
                      initDataArray._timeStamp, initDataArray._messageId);

      chat->addMessage(std::make_shared<Message>(message));

      changeLastReadIndexForSender(initDataArray._sender, chat);
    };
  } catch (const ValidationException &ex) {
    std::cout << " ! " << ex.what() << std::endl;
  }
}

/**
 * @brief Prompts the user to input a new message for a chat.
 * @param chatSystem Reference to the chat system.
 * @param chat Shared pointer to the chat.
 * @return True if a message was successfully added, false if the user cancels.
 * @throws EmptyInputException If the input is empty.
 */
bool inputNewMessage(ChatSystem &chatSystem, std::shared_ptr<Chat> chat) {
  std::cout << std::endl
            << "Наберите новое сообщение либо 0 для выхода:" << std::endl;
  std::string inputData;

  while (true) {
    try {
      std::getline(std::cin, inputData);

      if (inputData.empty())
        throw EmptyInputException();

      if (inputData == "0")
        return false;

      std::vector<std::shared_ptr<User>> recipients;
      for (const auto &participant : chat->getParticipants()) {
        auto user_ptr = participant._user.lock();
        if (user_ptr) {
          if (user_ptr != chatSystem.getActiveUser())
            recipients.push_back(user_ptr);
        }
      }

      InitDataArray newMessageStruct(inputData, getCurrentDateTime(),
                                     chatSystem.getActiveUser(), recipients, chatSystem.getNewMessageId());
      addMessageToChat(newMessageStruct, chat);
      return true;
    } // try
    catch (const ValidationException &ex) {
      std::cout << " ! " << ex.what() << " Попробуйте еще раз." << std::endl;
      continue;
    }
  } // while
}

/**
 * @brief Converts a string to lowercase.
 * @param str The input string.
 * @return The lowercase version of the input string.
 */
std::string TextToLower(const std::string &str) {
  std::string result = str;

  // приводим всех к нижнему регистру
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}
