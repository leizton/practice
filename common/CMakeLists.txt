cmake_minimum_required(VERSION 3.5)
include(../bin/tools.cmake)

set(COMMON_SOURCE_FILES "")
append_source_files(COMMON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(common STATIC ${COMMON_SOURCE_FILES})