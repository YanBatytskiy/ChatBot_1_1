cmake_minimum_required(VERSION 3.16)
project(ChatBot_1_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include paths — строго из проекта
include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/chat
  ${CMAKE_SOURCE_DIR}/src/ChatBot
  ${CMAKE_SOURCE_DIR}/src/exception
  ${CMAKE_SOURCE_DIR}/src/menu
  ${CMAKE_SOURCE_DIR}/src/message
  ${CMAKE_SOURCE_DIR}/src/system
  ${CMAKE_SOURCE_DIR}/src/user
)

# Все исходники
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Бинарник ChatBot_1_1
add_executable(ChatBot ${ALL_SOURCES})