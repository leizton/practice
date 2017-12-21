cmake_minimum_required(VERSION 3.5)
include("${PROJECT_SOURCE_DIR}/bin/tools.cmake")

set(CLIENT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/echo_client.cc")
add_executable(client ${CLIENT_SOURCE_FILES})
target_link_libraries(client common)

set(SERVER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/echo_server.cc")
add_executable(server ${SERVER_SOURCE_FILES})
target_link_libraries(server common)