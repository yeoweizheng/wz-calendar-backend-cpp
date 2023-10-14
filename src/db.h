#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include <sqlite_modern_cpp.h>
#include <models.h>

using namespace std;
using namespace sqlite;

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
string getJWTSecret();

#endif