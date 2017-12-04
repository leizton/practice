cmake_minimum_required(VERSION 3.5)
include(../bin/tools.cmake)

set(CLIENT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/echo_client.cc")
list(APPEND CLIENT_SOURCE_FILES ${COMMON_SOURCE_FILES})
add_executable(practice_epoll_client ${CLIENT_SOURCE_FILES})
target_link_libraries(practice_epoll_client pthread)

set(SERVER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/echo_server.cc")
list(APPEND SERVER_SOURCE_FILES ${COMMON_SOURCE_FILES})
add_executable(practice_epoll_server ${SERVER_SOURCE_FILES})
target_link_libraries(practice_epoll_server pthread)