#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <iostream>
#include <middlewares.h>
using namespace std;
using namespace crow;

extern App<AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();

// database structs
struct User {
    int id;
    string username;
    string password;
};

struct ScheduleItem {
    int id;
    string name;
    string date;
    int userId;
    bool done;
    int tagId;
};

// database methods
User getUser(string username);
vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId=0);
vector<ScheduleItem> getScheduleItems(int userId, string searchStr);
void insertScheduleItem(int userId, string name, string date, bool done, int tagId=0);
void updateScheduleItem(int userId, int scheduleItemId, string name, string date, bool done, int tagId=0);
void deleteScheduleItem(int userId, int scheduleItemId);

#endif
