#pragma once
#include <cstddef>
#include <string>
#include <vector>

enum class MessageContentType { Text, Image, File };

struct MessageContentDTO {
  MessageContentType _messageContentType;
  std::string payload;
};

struct MessageDTO {
  std::string _senderLogin;
  std::vector<MessageContentDTO> _messageContent;
  std::string _timeStamp;
  std::size_t _messageId;
};

struct ChatDTO {
  std::size_t _chatId;
  std::vector<std::string> _participantLogins;
  std::string _lastMessageTime;
  std::size_t unreadCount;
};