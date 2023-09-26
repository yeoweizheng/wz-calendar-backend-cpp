cmake_minimum_required(VERSION 3.10)
project(wzcalendar)

add_executable(wzcalendar
    src/app.cpp
    src/routes/user.cpp
    src/db/db.cpp
)
target_include_directories(wzcalendar PRIVATE src/)

find_package(Crow)
target_link_libraries(wzcalendar PUBLIC Crow::Crow)

target_include_directories(wzcalendar PRIVATE libs/sqlite_modern_cpp/hdr)
target_link_libraries(wzcalendar PRIVATE sqlite3)