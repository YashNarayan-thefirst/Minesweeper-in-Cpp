cmake_minimum_required(VERSION 3.29)
project(minesweeper)
set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "your-sfml-directory-here") 
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
include_directories(${CMAKE_SOURCE_DIR})
set(SOURCES
        Button.cpp
        GameState.cpp
        Tile.cpp
        Toolbox.cpp
        minesweeper.cpp
)
set(HEADERS
        Button.h
        GameState.h
        minesweeper.h
        Tile.h
        Toolbox.h
)

add_executable(minesweeper ${SOURCES} ${HEADERS})
target_link_libraries(minesweeper sfml-graphics)
