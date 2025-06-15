#pragma once
#include "message/message_content.h"
#include "user/user.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Interface for messages.
 */
class IMessage { // interface for messages
public:
  /**
   * @brief Default virtual destructor.
   */
  virtual ~IMessage() = default;
};

/**
 * @brief Class representing a message with various content types.
 */
class Message : public IMessage {
private:
  std::vector<std::shared_ptr<IMessageContent>> _content; ///< Vector of message content.
  std::weak_ptr<User> _sender;                            ///< Sender of the message.
  std::string _time_stamp;                                ///< Timestamp of the message (to be implemented).

public:
  /**
   * @brief Constructor for Message.
   * @param content Vector of shared pointers to message content.
   * @param sender Weak pointer to the sender user.
   * @param timeStamp Timestamp of the message.
   */
  Message(const std::vector<std::shared_ptr<IMessageContent>> &content, const std::weak_ptr<User> &sender,
          const std::string &timeStamp);

  /**
   * @brief Default destructor.
   */
  ~Message() override = default; // destructor

  /**
   * @brief Gets the content of the message.
   * @return Const reference to the vector of message content.
   */
  const std::vector<std::shared_ptr<IMessageContent>> &getContent() const;

  /**
   * @brief Gets the sender of the message.
   * @return Weak pointer to the sender user.
   */
  std::weak_ptr<User> getSender() const;

  /**
   * @brief Gets the timestamp of the message.
   * @return Const reference to the timestamp string.
   */
  const std::string &getTimeStamp() const;

  /**
   * @brief Adds content to the message.
   * @param content Shared pointer to the content to be added.
   */
  void addContent(const std::shared_ptr<IMessageContent> &content);

  /**
   * @brief Prints the message for a specific user.
   * @param currentUser Shared pointer to the user viewing the message.
   * @note Contains a typo in the parameter name (¤tUser).
   */
  void printMessage(const std::shared_ptr<User> &currentUser);

  // add content to the message
  // delete message will be realized further

  // edit message will be realized further
};
