#pragma once

#include "message/message.h"
#include "system/weak_map.h"
#include "user/user.h"
#include <memory>
#include <vector>

/**
 * @struct Participant
 * @brief Represents a participant in a chat.
 */
struct Participant {
  std::weak_ptr<User> _user; ///< Weak reference to the user.
                             ///< participant.
  bool _deletedFromChat;     ///< Indicates if the participant was removed from the
                             ///< chat.
};

/**
 * @class Chat
 * @brief Manages a chat system with participants and messages.
 */
class Chat {
private:
  std::vector<Participant> _participants;          ///< List of chat participants.
  std::vector<std::shared_ptr<Message>> _messages; ///< List of messages in the chat.
  weak_map<User, std::size_t> _lastReadMessageMap;
  std::size_t _chatId;

public:
  /**
   * @brief Default constructor for an empty chat.
   * @details The message vector is automatically initialized as empty by std::vector.
   */
  Chat() = default;
  Chat(const Chat &) = delete;
  Chat &operator=(const Chat &) = delete;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~Chat() = default;

  /**
   * @brief Assigns a unique ID to the chat.
   * @param chatId Chat ID to assign.
   */
  void addChatId(std::size_t chatId);

  /**
   * @brief Adds a new participant to the chat.
   * @param user Shared pointer to the user to be added.
   */
  void addParticipant(const std::shared_ptr<User> &user);

  /**
   * @brief Adds a message to the chat.
   * @param message Shared pointer to the message to be added.
   */
  void addMessage(const std::shared_ptr<Message> &message);

  /**
   * @brief Marks a user as deleted from the chat.
   * @param user Shared pointer to the user.
   */
  void setDeletedFromChat(const std::shared_ptr<User> &user);

  /**
   * @brief Retrieves the chatId.
   * @return Constant reference to the chat ID.
   */
  const std::size_t &getChatId() const;

  /**
   * @brief Retrieves the list of messages in the chat.
   * @return Constant reference to the vector of messages.
   */
  const std::vector<std::shared_ptr<Message>> &getMessages() const;

  /**
   * @brief Retrieves the list of participants in the chat.
   * @return Constant reference to the vector of participants.
   */
  const std::vector<Participant> &getParticipants() const;

  /**
   * @brief Gets the index of the last message read by a specific user.
   * @param user Shared pointer to the user.
   * @return The index of the last read message.
   */
  std::size_t getLastReadMessageIndex(const std::shared_ptr<User> &user) const;

  /**
   * @brief Checks if a user has been marked as deleted from the chat.
   * @param user Shared pointer to the user.
   * @return True if the user is deleted, false otherwise.
   */
  bool getDeletedFromChat(const std::shared_ptr<User> &user) const;

  /**
   * @brief Prints the full chat contents for a specific user.
   * @param currentUser Shared pointer to the user viewing the chat.
   */
  void printChat(const std::shared_ptr<User> &currentUser);

  /**
   * @brief Updates the last read message index for a specific user.
   * @param user Shared pointer to the user.
   * @param newLastReadMessageIndex New index to store.
   */
  void updateLastReadMessageIndex(const std::shared_ptr<User> &user, std::size_t newLastReadMessageIndex);
};
