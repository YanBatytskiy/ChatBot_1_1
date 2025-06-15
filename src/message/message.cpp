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
                 const std::string &timeStamp)
    : _content(content), _sender(sender), _time_stamp(timeStamp) {}

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
 * @brief Prints the message for a specific user.
 * @param currentUser Shared pointer to the user viewing the message.
 * @details Displays the message with formatting based on whether it is incoming or outgoing, including sender details
 * and content.
 * @note Contains a typo in the parameter name (¤tUser).
 */
void Message::printMessage(const std::shared_ptr<User> &currentUser) {
  auto sender = _sender.lock();
  if (!sender)
    return;

  bool messageDirection = (sender == currentUser);

  if (!messageDirection) { // income Message

    std::cout << "\033[32m"; // red
    auto sender_ptr = _sender.lock();
    if (sender_ptr) {
    } else {
      std::cout << "user уничтожен " << sender_ptr->getLogin() << std::endl;
    }
    std::cout << "     -> Входящее от Логин/Имя:    " << sender_ptr->getLogin() << "/" << sender_ptr->getUserName()
              << "    " << _time_stamp << std::endl;

    for (const auto &content : _content) {
      if (auto textContent = std::dynamic_pointer_cast<MessageContent<TextContent>>(content)) {
        std::cout << textContent->getMessageContent()._text << std::endl;
      } else if (auto imageContent = std::dynamic_pointer_cast<MessageContent<ImageContent>>(content)) {
        std::cout << imageContent->getMessageContent()._image << std::endl;
      }
    }
  } else {

    std::cout << "\033[37m"; // white
    std::cout << "<- Исходящее от тебя: " << currentUser->getUserName() << "    " << _time_stamp << std::endl;

    for (const auto &content : _content) {
      if (auto textContent = std::dynamic_pointer_cast<MessageContent<TextContent>>(content)) {
        std::cout << textContent->getMessageContent()._text << std::endl;
      } else if (auto imageContent = std::dynamic_pointer_cast<MessageContent<ImageContent>>(content)) {
        std::cout << imageContent->getMessageContent()._image << std::endl;
      }
    }
  }

  std::cout << "\033[0m";
};

// delete message will be realized further

// edit message will be realized further
