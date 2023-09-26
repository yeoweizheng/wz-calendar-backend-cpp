cmake_minimum_required(VERSION 3.10)

project(wzcalendar)

add_executable(wzcalendar src/app.cpp)

find_package(Crow)
target_link_libraries(wzcalendar PUBLIC Crow::Crow)