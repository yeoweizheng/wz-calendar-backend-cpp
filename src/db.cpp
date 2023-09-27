#include <main.h>
#include <sqlite_modern_cpp.h>
#include <constants.h>
using namespace sqlite;

User getUserByUsername(string username) {
    database db(DB_FILENAME);
    User user;
    db << "SELECT id, username, password FROM auth_user WHERE username = ?;" << username
        >> [&](int userId, string username, string password) {
            user.id = userId;
            user.username = username;
            user.password = password;
        };
    return user;
}

vector<ScheduleItem> getScheduleItemsByUserIdAndDates(int userId, string startDate, string endDate, int tagId) {
    database db(DB_FILENAME);
    vector<ScheduleItem> scheduleItems;
    auto populateScheduleItems = [&](int id, string name, string date, bool done, int tagId) {
        ScheduleItem scheduleItem;
        scheduleItem.id = id;
        scheduleItem.name = name;
        scheduleItem.date = date;
        scheduleItem.done = done;
        scheduleItem.tagId = tagId;
        scheduleItems.push_back(scheduleItem);
    };
    if (tagId == 0) {
        db << "SELECT id, name, date, done, tag_id FROM schedule_scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?);"
            << userId << startDate << endDate
            >> populateScheduleItems;
    } else if (tagId == -1) {
            db << "SELECT id, name, date, done, tag_id FROM schedule_scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?) AND tag_id IS NULL;"
                << userId << startDate << endDate
                >> populateScheduleItems;
    } else {
        db << "SELECT id, name, date, done, tag_id FROM schedule_scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?) AND tag_id = ?;"
            << userId << startDate << endDate << tagId
            >> populateScheduleItems;
    }
    return scheduleItems;
}

void insertScheduleItem(int userId, string name, string date, bool done, int tagId) {
    database db(DB_FILENAME);
    if (tagId == 0) {
        db << "INSERT INTO schedule_scheduleitem (user_id, name, date, done) VALUES (?, ?, ?, ?);"
            << userId << name << date << done;
    } else {
        db << "INSERT INTO schedule_scheduleitem (user_id, name, date, done, tag_id) VALUES (?, ?, ?, ?, ?);"
            << userId << name << date << done << tagId;
    }
}