set(CMAKE_CXX_STANDARD 11)

add_definitions(-DARDUINO=20300)
add_definitions(-DLWIP_OPEN_SRC)
add_definitions(-DF_CPU=80000000L)
add_definitions(-D__ets__)
add_definitions(-DICACHE_FLASH)
add_definitions(-DPLATFORMIO=30400)
add_definitions(-DESP8266)
add_definitions(-DARDUINO_ARCH_ESP8266)
add_definitions(-DARDUINO_ESP8266_ESP12)
add_definitions(-DMQTT_MAX_PACKET_SIZE=256)

include_directories(${CMAKE_SOURCE_DIR}/.piolibdeps/Streaming_ID560)

include_directories(mock)

add_executable(HardwareTests
        ${CMAKE_CURRENT_SOURCE_DIR}/mock/Arduino.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mock/ArduinoMock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mock/HardwareSerialMock.cpp
        ${CMAKE_SOURCE_DIR}/src/Smarty/MqttPublication.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MqttPublicationTest.cpp
        ${CMAKE_SOURCE_DIR}/src/Smarty/MqttSubscription.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MqttSubscriptionTest.cpp
        ${CMAKE_SOURCE_DIR}/src/Smarty/Uptime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UptimeTest.cpp
)

target_link_libraries(HardwareTests gtest gtest_main)
target_link_libraries(HardwareTests gmock gmock_main)
