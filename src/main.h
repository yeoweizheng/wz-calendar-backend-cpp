#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <iostream>
#include <sqlite_modern_cpp.h>
#include <jwt-cpp/jwt.h>
#include <bcrypt/BCrypt.hpp>
#include <middlewares.h>
using namespace std;
using namespace crow;

extern App<AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();

// database structs
struct User {
    string username;
    string password;
};

// database methods
User getUserByUsername(string username);

#endif
