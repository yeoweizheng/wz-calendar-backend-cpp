#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <iostream>
#include <middlewares.h>
#include <models.h>
#include <sqlite_modern_cpp.h>
using namespace std;
using namespace crow;
using namespace sqlite;

extern App<CORSMiddleware, AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();
void setupTagRoutes();

// database instance
extern unique_ptr<database> db;

// database methods
User getUser(string username);
User getUser(int userId);
void updateUser(int userId, string username, string password);
vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId=0);
vector<ScheduleItem> getScheduleItems(int userId, string searchStr);
ScheduleItem getScheduleItem(int userId, int scheduleItemId);
int insertScheduleItem(int userId, string name, string date, string time, bool done, int tagId);
void updateScheduleItem(int userId, int scheduleItemId, string name, string date, string time, bool done, int tagId);
void deleteScheduleItem(int userId, int scheduleItemId);
vector<Tag> getTags(int userId);
Tag getTag(int userId, int tagId);
int insertTag(int userId, string name);
void updateTag(int userId, int tagId, string name);
void deleteTag(int userId, int tagId);
void checkTagExists(int userId, int tagId);

// utils
json::wvalue structToWValue(ScheduleItem scheduleItem);
json::wvalue structToWValue(Tag tag);

#endif
