#pragma once

#include "message/message.h"
#include "user/user.h"
#include <memory>
#include <vector>

/**
 * @struct Participant
 * @brief Represents a participant in a chat.
 */
struct Participant {
  std::weak_ptr<User> _user;         ///< Weak reference to the user.
  std::size_t _lastReadMessageIndex; ///< Index of the last message read by the participant.
  bool _deletedFromChat;             ///< Indicates if the participant was removed from the chat.
};

/**
 * @class Chat
 * @brief Manages a chat system with participants and messages.
 */
class Chat {
private:
  std::vector<Participant> _participants;          ///< List of chat participants.
  std::vector<std::shared_ptr<Message>> _messages; ///< List of messages in the chat.

public:
  /**
   * @brief Default constructor for an empty chat.
   * @details The message vector is automatically initialized as empty by std::vector.
   */
  Chat() = default;
  Chat(const Chat &) = delete;
  Chat& operator = (const Chat&) = delete;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~Chat() = default;

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
   * @brief Sets the deletedFromChat status for a specific user.
   * @param user Shared pointer to the user.
   * @param deletedFromChat Boolean indicating if the user is deleted from the chat.
   */
  void setDeletedFromChat(const std::shared_ptr<User> &user, const bool deletedFromChat);

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
   * @brief Checks if a user has been deleted from the chat.
   * @param user Shared pointer to the user.
   * @return True if the user is deleted from the chat, false otherwise.
   */
  bool getDeletedFromChat(const std::shared_ptr<User> &user) const;

  /**
   * @brief Prints the full chat for a specific user.
   * @param currentUser Shared pointer to the user viewing the chat.
   */
  void printChat(const std::shared_ptr<User> &currentUser);

  /**
   * @brief Updates the last read message index for a specific user.
   * @param user Shared pointer to the user.
   * @param newLastReadMessageIndex The new index of the last read message.
   */
  void updateLastReadMessageIndex(const std::shared_ptr<User> &user, std::size_t newLastReadMessageIndex);

  /**
   * @brief Removes a participant from the chat (to be implemented).
   * @param user Shared pointer to the user to be removed.
   */
  // void removeParticipant(const std::shared_ptr<User> &user);
};