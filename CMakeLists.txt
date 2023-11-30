cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall -g -pThread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Raiton)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} src)

add_executable(Raiton src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/entity.cpp src/player.cpp src/userSettings.cpp src/playerBullet.cpp src/enemy.cpp src/enemyBullet.cpp src/stage.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(Raiton ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} Threads::Threads)
