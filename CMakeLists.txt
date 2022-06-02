cmake_minimum_required(VERSION 3.20)
project(Glee-no-mess)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,leak,undefined")

#set(CMAKE_PREFIX_PATH  "/opt/Qt/6.3.0/gcc_64/")

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
include_directories(model/items)
include_directories(model/guest)
include_directories(model/level)
include_directories(model/main_character)
include_directories(tools)
include_directories(view)
include_directories(controller)
include_directories(resources)
include_directories(model/casino)

file(GLOB CONTROLLER controller/*)
file(GLOB VIEW view/*)
file(GLOB RESOURCES resources/*)
file(GLOB MODEL model/*)
file(GLOB MODEL_MAINCHARACTER model/main_character/*)
file(GLOB MODEL_GUEST model/guest/*)
file(GLOB MODEL_LEVEL model/level/*)
file(GLOB TOOLS tools/*)
file(GLOB MODEL_ITEMS model/items/*)
file(GLOB MODEL_CASINO model/casino/*)

add_executable(default-target
        ${CONTROLLER}
        ${RESOURCES}
        ${VIEW}
        ${MODEL}
        ${MODEL_MAINCHARACTER}
        ${MODEL_GUEST}
        ${MODEL_LEVEL}
        ${TOOLS}
        ${MODEL_ITEMS}
        ${MODEL_CASINO}
        main.cpp)

target_link_libraries(default-target
        Qt::Core
        Qt::Gui
        Qt::Widgets)
