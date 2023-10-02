cmake_minimum_required(VERSION 3.10)
project(wzcalendar)

add_executable(wzcalendar
    src/main.cpp
    src/routes.cpp
    src/db.cpp
    src/utils.cpp
)
target_include_directories(wzcalendar PRIVATE src/)
find_package(Crow)
target_link_libraries(wzcalendar PRIVATE Crow::Crow)
target_include_directories(wzcalendar PRIVATE libs/sqlite_modern_cpp/hdr)
target_link_libraries(wzcalendar PRIVATE sqlite3)
target_include_directories(wzcalendar PRIVATE libs/jwt-cpp/include)
target_link_libraries(wzcalendar PRIVATE ssl)
target_link_libraries(wzcalendar PRIVATE crypto)
target_link_libraries(wzcalendar PRIVATE bcrypt)

add_executable(manageusers src/manageusers.cpp)
target_include_directories(manageusers PRIVATE libs/sqlite_modern_cpp/hdr)
target_link_libraries(manageusers PRIVATE sqlite3)
target_link_libraries(manageusers PRIVATE crypto)
target_link_libraries(manageusers PRIVATE bcrypt)