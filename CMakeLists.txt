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

include_directories(model)
include_directories(model/guest)
include_directories(model/level)
include_directories(model/main_character)
include_directories(model/tools)
include_directories(view)
include_directories(controller)
include_directories(resources)

file(GLOB_RECURSE CONTROLLER controller/*)
file(GLOB_RECURSE VIEW view/*)
file(GLOB_RECURSE RESOURCES resources/*)
file(GLOB_RECURSE MODEL model/*)

add_executable(default-target
        ${CONTROLLER}
        ${RESOURCES}
        ${VIEW}
        ${MODEL}
        main.cpp
        model/tools/mvc.h
        )

target_link_libraries(default-target
        Qt::Core
        Qt::Gui
        Qt::Widgets)
