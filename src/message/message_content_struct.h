#pragma once
#include "user/user.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Structure to hold initial data for a chat message.
 */
struct InitDataArray {
  std::string _messageText;                       ///< Text content of the message.
  std::string _timeStamp;                         ///< Timestamp of the message.
  std::shared_ptr<User> _sender;                  ///< Sender of the message.
  std::vector<std::shared_ptr<User>> _recipients; ///< List of message recipients.

  /**
   * @brief Constructor for InitDataArray.
   * @param messageText Text content of the message.
   * @param timeStamp Timestamp of the message.
   * @param sender Shared pointer to the sender user.
   * @param _recipients Vector of shared pointers to recipient users.
   */
  InitDataArray(std::string messageText, std::string timeStamp, std::shared_ptr<User> sender,
                std::vector<std::shared_ptr<User>> _recipients);

  /**
   * @brief Default destructor.
   */
  virtual ~InitDataArray() = default;
};

/**
 * @brief Structure for text content in a message.
 */
struct TextContent {
  std::string _text; ///< Text content.

  /**
   * @brief Constructor for TextContent.
   * @param text The text content.
   */
  TextContent(const std::string &text) : _text(text) {};

  /**
   * @brief Default destructor.
   */
  ~TextContent() = default;
};

/**
 * @brief Structure for file content in a message (to be implemented).
 */
struct FileContent { // will be realized further
  std::string _fileName; ///< Name of the file.

  /**
   * @brief Constructor for FileContent.
   * @param fileName Name of the file.
   */
  FileContent(const std::string &fileName) : _fileName(fileName) {};

  /**
   * @brief Default destructor.
   */
  ~FileContent() = default;
};

/**
 * @brief Structure for image content in a message (to be implemented).
 */
struct ImageContent { // will be realized further
  std::string _image; ///< Image identifier or path.

  /**
   * @brief Constructor for ImageContent.
   * @param image Image identifier or path.
   */
  ImageContent(const std::string &image) : _image(image) {};

  /**
   * @brief Default destructor.
   */
  ~ImageContent() = default;
};

