#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <iostream>
#include <middlewares.h>
#include <models.h>
using namespace std;
using namespace crow;

extern App<CORSMiddleware, AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();
void setupTagRoutes();

// database methods
User getUser(string username);
vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId=0);
vector<ScheduleItem> getScheduleItems(int userId, string searchStr);
ScheduleItem getScheduleItem(int userId, int scheduleItemId);
int insertScheduleItem(int userId, string name, string date, bool done, int tagId);
void updateScheduleItem(int userId, int scheduleItemId, string name, string date, bool done, int tagId);
void deleteScheduleItem(int userId, int scheduleItemId);
vector<Tag> getTags(int userId);
Tag getTag(int userId, int tagId);
int insertTag(int userId, string name);
void updateTag(int userId, int tagId, string name);
void deleteTag(int userId, int tagId);

// utils
json::wvalue structToWValue(ScheduleItem scheduleItem);
json::wvalue structToWValue(Tag tag);

#endif
