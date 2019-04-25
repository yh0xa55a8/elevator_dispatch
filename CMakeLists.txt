cmake_minimum_required(VERSION 3.13)
project(OS_Assignment1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/Users/yh/Qt5.11.2/5.11.2/clang_64/lib/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

set(LINK_LIBS Qt5::Widgets)

set(SOURCES
        main.cpp
        MainWindowController.cpp
        MainWindowController.h
        InternalPanel.cpp
        InternalPanel.h
        StatusButton.cpp
        StatusButton.h
        ExternalPanel.cpp
        ExternalPanel.h Dispatcher.cpp Dispatcher.h Elevator.cpp Elevator.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries( ${PROJECT_NAME} ${LINK_LIBS})