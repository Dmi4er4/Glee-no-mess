cmake_minimum_required(VERSION 3.21)
project(Glee-no-mess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(default-target

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
        visitor.h
        level.cpp
        level.h typedefs.h BadCharacteristic.cpp BadCharacteristic.h random.cpp random.h tools.h)

target_link_libraries(default-target

        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(only_main main.cpp random.cpp random.h)
