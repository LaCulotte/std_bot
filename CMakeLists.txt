cmake_minimum_required(VERSION 3.0)

project(test_lib)

set(CMAKE_BUILD_TYPE Debug)

set(SRCS
    std_bot/logger/Logger.cpp
    std_bot/dataContainers/MessageDataBuffer.cpp
    std_bot/messaging/MessageInterface.cpp
    std_bot/connection/NetworkConnection.cpp
    std_bot/test_class/RawMessage.cpp
    std_bot/test_class/ReadFrame.cpp
    test_lib.cpp
)

add_executable(test_lib ${SRCS})

target_include_directories(test_lib PUBLIC ./std_bot/std_include)
target_include_directories(test_lib PUBLIC ./std_bot/dataContainers)
target_include_directories(test_lib PUBLIC ./std_bot/logger)
target_include_directories(test_lib PUBLIC ./std_bot/messaging)
target_include_directories(test_lib PUBLIC ./std_bot/test_class)
target_include_directories(test_lib PUBLIC ./std_bot/frames)
target_include_directories(test_lib PUBLIC ./std_bot/connection)

target_link_libraries(test_lib pthread)
target_link_libraries(test_lib stdc++fs)

set_property(TARGET test_lib PROPERTY CXX_STANDARD 20)