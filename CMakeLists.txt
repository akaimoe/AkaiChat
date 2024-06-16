cmake_minimum_required(VERSION 3.5)

project(ChatApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(ChatApp
    main.cpp
    chatdialog.cpp
    chatdialog.h
    chatdialog.ui
)

target_link_libraries(ChatApp Qt5::Widgets Qt5::Network)

