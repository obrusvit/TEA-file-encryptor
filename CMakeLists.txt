cmake_minimum_required(VERSION 3.16)
project(TEAFileEncryptor VERSION 1.0)

add_definitions("-std=c++2a")

# create compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

# add dependencies using CPM
include(cmake/CPM.cmake)
CPMAddPackage("gh:catchorg/Catch2@3.4.0")
CPMAddPackage("gh:bfgroup/Lyra#1.6.1")

add_executable(TEA-file-encryptor src/main.cpp)
target_link_libraries(TEA-file-encryptor PRIVATE bfg::lyra)

add_executable(tests test/run_tests.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
# Define the path to the assets to be accessible from test directory with relative path
target_compile_definitions(tests PRIVATE ASSET_PATH="${CMAKE_SOURCE_DIR}/test/assets/")

