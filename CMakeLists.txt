cmake_minimum_required(VERSION 3.10)

project(Grox_Music_Player)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MPDCLIENT REQUIRED libmpdclient)

add_executable(groxmusicplayer main.cpp musicplayer.cpp GUI.cpp)

target_include_directories(groxmusicplayer PUBLIC ${MPDCLIENT_INCLUDE_DIRS})

target_link_libraries(groxmusicplayer ${MPDCLIENT_LIBRARIES})

message(STATUS "Found libmpdclient version: ${MPDCLIENT_VERSION}")