#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <iostream>
#include <middlewares.h>
#include <models.h>
using namespace std;
using namespace crow;

extern App<AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();

// database methods
User getUser(string username);
vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId=0);
vector<ScheduleItem> getScheduleItems(int userId, string searchStr);
ScheduleItem getScheduleItem(int userId, int scheduleItemId);
int insertScheduleItem(int userId, string name, string date, bool done, int tagId);
void updateScheduleItem(int userId, int scheduleItemId, string name, string date, bool done, int tagId);
void deleteScheduleItem(int userId, int scheduleItemId);

// utils
json::wvalue structToWValue(ScheduleItem scheduleItem);

#endif
