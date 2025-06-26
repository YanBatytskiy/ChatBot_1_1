cmake_minimum_required(VERSION 3.16)
project(ChatBot)

if(DEFINED SOURCE_FILE)
  get_filename_component(EXEC_NAME "${SOURCE_FILE}" NAME_WE)
  message(STATUS "Building single file: ${SOURCE_FILE} → ${EXEC_NAME}")

  include_directories("${CMAKE_SOURCE_DIR}/src")

  file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

  set(MAIN_FILE "${CMAKE_SOURCE_DIR}/${SOURCE_FILE}")
  list(REMOVE_ITEM ALL_SOURCES "${MAIN_FILE}")

  add_executable(${EXEC_NAME} "${MAIN_FILE}" ${ALL_SOURCES})

  return()
endif()
