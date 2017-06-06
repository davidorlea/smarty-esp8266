cmake_minimum_required(VERSION 3.2)
project(Hardware)

set(PLATFORMIO_CMD "/usr/local/bin/platformio")

include_directories(lib)
include_directories(src)
include_directories(test)

add_subdirectory(lib/gtest)
add_subdirectory(src)
add_subdirectory(test)

add_custom_target(
        PLATFORMIO_BUILD ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_UPLOAD ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_CLEAN ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_TEST ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion test
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_PROGRAM ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_UPLOADFS ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        PLATFORMIO_UPDATE_ALL ALL
        COMMAND ${PLATFORMIO_CMD} -f -c clion update
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
