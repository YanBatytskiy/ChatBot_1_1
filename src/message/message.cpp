#include "message/message.h"
#include "message/message_content.h"
#include "message/message_content_struct.h"
#include <iostream>
#include <memory>

/**
 * @brief Constructor for Message.
 * @param content Vector of shared pointers to message content.
 * @param sender Weak pointer to the sender user.
 * @param timeStamp Timestamp of the message.
 */
Message::Message(const std::vector<std::shared_ptr<IMessageContent>> &content, const std::weak_ptr<User> &sender,
                 const std::string &timeStamp, std::size_t messageId)
    : _content(content), _sender(sender), _time_stamp(timeStamp), _messageId(messageId) {}

/**
 * @brief Retrieves the messageId.
 */
const std::size_t &Message::getMessagetId() const { return _messageId; }

/**
 * @brief Gets the content of the message.
 * @return Const reference to the vector of message content.
 */
const std::vector<std::shared_ptr<IMessageContent>> &Message::getContent() const { return _content; }

/**
 * @brief Gets the sender of the message.
 * @return Weak pointer to the sender user.
 */
std::weak_ptr<User> Message::getSender() const { return _sender; }

/**
 * @brief Gets the timestamp of the message.
 * @return Const reference to the timestamp string.
 */
const std::string &Message::getTimeStamp() const { return _time_stamp; }

/**
 * @brief Adds content to the message.
 * @param content Shared pointer to the content to be added.
 */
void Message::addContent(const std::shared_ptr<IMessageContent> &content) { _content.push_back(content); }

/**
 * @brief Adds messageId to the message.
 */
void Message::addMessageId(std::size_t messageId) { _messageId = messageId; }

/**
 * @brief Prints the message for a specific user.
 * @param currentUser Shared pointer to the user viewing the message.
 * @details Displays the message with formatting based on whether it is incoming or outgoing, including sender details
 * and content.
 * @note Contains a typo in the parameter name (¤tUser).
 */
void Message::printMessage(const std::shared_ptr<User> &currentUser) {

  // определили отправителя
  auto sender_ptr = _sender.lock();

  // определили направление сообщения
  bool messageDirection;

  if (sender_ptr == nullptr) {
    messageDirection = false;
  } else
    messageDirection = (sender_ptr == currentUser);

  if (!messageDirection) { // income Message

    std::cout << "\033[32m"; // green

    if (sender_ptr != nullptr) {
      std::cout << "     <- Входящее от Логин/Имя:    " << sender_ptr->getLogin() << "/" << sender_ptr->getUserName()
                << "    " << _time_stamp << ", messageId: " << _messageId << std::endl;
    } else {
      std::cout << "     <- Входящее от Логин/Имя:    " << "Пользователь удален.    " << _time_stamp
                << ", messageId: " << _messageId << std::endl;
    }
  } else {
    std::cout << "\033[37m"; // white
    std::cout << "-> Исходящее от тебя: " << currentUser->getUserName() << "    " << _time_stamp
              << " messageId: " << _messageId << std::endl;
  }
  for (const auto &content : _content) {
    if (auto textContent = std::dynamic_pointer_cast<MessageContent<TextContent>>(content)) {
      std::cout << textContent->getMessageContent()._text << std::endl;
    } else if (auto imageContent = std::dynamic_pointer_cast<MessageContent<ImageContent>>(content)) {
      std::cout << imageContent->getMessageContent()._image << std::endl;
    }
  }

  std::cout << "\033[0m";
}