if(DEFINED SOURCE_FILE)
    get_filename_component(EXEC_NAME "${SOURCE_FILE}" NAME_WE)
    message(STATUS "Building single file: ${SOURCE_FILE} → ${EXEC_NAME}")

    include_directories(${CMAKE_SOURCE_DIR}/src)

    file(GLOB_RECURSE OTHER_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
    list(REMOVE_ITEM OTHER_SOURCES ${CMAKE_SOURCE_DIR}/${SOURCE_FILE})

    add_executable(${EXEC_NAME} ${CMAKE_SOURCE_DIR}/${SOURCE_FILE} ${OTHER_SOURCES})
    return()
endif()
