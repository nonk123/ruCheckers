cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(ruCheckers LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(STEAMWORKS_SDK_ZIP ${CMAKE_SOURCE_DIR}/steamworks_sdk_162.zip)

include(FetchContent)

FetchContent_Declare(
    caulk
    GIT_REPOSITORY https://github.com/Schwungus/caulk.git
    GIT_TAG master) # TODO: pin to a specific release

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5)

FetchContent_MakeAvailable(caulk raylib)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c)
file(GLOB_RECURSE HEADERS ${SRC_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
caulk_register(${PROJECT_NAME})
