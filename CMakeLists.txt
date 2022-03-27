cmake_minimum_required(VERSION 3.21)
project(Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(
        Project

        main.cpp
        model.cpp
        model.h
        view.cpp
        view.h
        controller.cpp
        controller.h
        main_character.cpp
        main_character.h
        visitor.cpp
        visitor.h)

target_link_libraries(
        Project

        Qt::Core
        Qt::Gui
        Qt::Widgets)
