#pragma once
#include "client-server/chat_system.h"
#include "system/dto_struct.h"
#include <cstddef>
#include <vector>

class ClientSession {
private:
  ChatSystem &_server; // link to server
  std::shared_ptr<User> _clientUser;

public:
  // constructors
  ClientSession(ChatSystem &server);

  // getters

  // setters
};