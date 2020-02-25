cmake_minimum_required(VERSION 3.14)

project(jquiz)

set(CMAKE_CXX_STANDARD 20)

add_executable(jquiz src/main.cpp)
add_executable(dbtool src/dbtool.cpp)

include_directories(./include)

find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

target_link_libraries(jquiz SQLite::SQLite3)
target_link_libraries(dbtool SQLite::SQLite3)

configure_file(${PROJECT_SOURCE_DIR}/data.db ${PROJECT_SOURCE_DIR}/build/data.db COPYONLY)
