/**
 * @file code_init_system.h
 * @brief Заголовочный файл для автоматической инициализации тестовой системы чатов и сообщений.
 *
 * Содержит структуру данных для описания сообщений и функции для создания тестовых пользователей,
 * чатов и сообщений в системе ChatBot.
 */

#pragma once

#include "message/message_content_struct.h"
#include "system/chat_system.h"
#include <memory>

void addMessageToChat(const InitDataArray &initDataArray, std::shared_ptr<Chat> &chat);

/**
 * @brief Инициализирует тестовую систему: создаёт пользователей, чаты и сообщения.
 *
 * @param _chatsystem Система чатов для заполнения тестовыми данными
 */
void systemInitTest(ChatSystem &_chatsystem);
