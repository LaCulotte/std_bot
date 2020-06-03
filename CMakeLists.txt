cmake_minimum_required(VERSION 3.0)

project(test_lib)

set(CMAKE_BUILD_TYPE Debug)

set(SRCS
    std_bot/logger/Logger.cpp
    std_bot/dataContainers/MessageDataBuffer.cpp
    std_bot/messaging/MessageInterface.cpp
    std_bot/connection/NetworkConnection.cpp
    std_bot/connection/PrefixNetworkConnection.cpp
    std_bot/connection/ClientConnection.cpp
    std_bot/connection/ListeningServer.cpp
    std_bot/frames/connectionFrames/BasicConnectionFrame.cpp
    std_bot/units/WorkingUnit.cpp
    std_bot/units/MessagingUnit.cpp
    std_bot/units/ConnectionUnit.cpp
    std_bot/test_class/RawMessage.cpp
    std_bot/test_class/ReadFrame.cpp
    std_bot/test_class/OtherUselessFrame.cpp
    std_bot/test_class/BasicPrefixConnection.cpp
    std_bot/test_class/BasicPrefixedMessage.cpp
    std_bot/test_class/BasicMessageHandler.cpp
    std_bot/test_class/ConnectionManagerFrame.cpp
    std_bot/std_include/socket_include.cpp
    test_lib.cpp
)

add_executable(test_lib ${SRCS})

target_include_directories(test_lib PUBLIC ./std_bot/std_include)
target_include_directories(test_lib PUBLIC ./std_bot/dataContainers)
target_include_directories(test_lib PUBLIC ./std_bot/logger)
target_include_directories(test_lib PUBLIC ./std_bot/messaging)
target_include_directories(test_lib PUBLIC ./std_bot/messaging/connectionMessages)
target_include_directories(test_lib PUBLIC ./std_bot/test_class)
target_include_directories(test_lib PUBLIC ./std_bot/frames)
target_include_directories(test_lib PUBLIC ./std_bot/frames/connectionFrames)
target_include_directories(test_lib PUBLIC ./std_bot/connection)
target_include_directories(test_lib PUBLIC ./std_bot/units)

target_link_libraries(test_lib pthread)
target_link_libraries(test_lib stdc++fs)

set_property(TARGET test_lib PROPERTY CXX_STANDARD 20)