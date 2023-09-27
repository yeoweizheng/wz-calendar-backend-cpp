#include <main.h>
#include <sqlite_modern_cpp.h>
#include <constants.h>
using namespace sqlite;

User getUser(string username) {
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

vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId) {
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

vector<ScheduleItem> getScheduleItems(int userId, string searchStr) {
    database db(DB_FILENAME);
    vector<ScheduleItem> scheduleItems;
    searchStr = "%" + searchStr + "%";
    db << "SELECT id, name, date, done, tag_id FROM schedule_scheduleitem WHERE user_id = ? AND done = false AND name LIKE ?;"
        << userId << searchStr
        >> [&](int id, string name, string date, bool done, int tagId) {
            ScheduleItem scheduleItem;
            scheduleItem.id = id;
            scheduleItem.name = name;
            scheduleItem.date = date;
            scheduleItem.done = done;
            scheduleItem.tagId = tagId;
            scheduleItems.push_back(scheduleItem);
        };
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

void updateScheduleItem(int userId, int scheduleItemId, string name, string date, bool done, int tagId) {
    database db(DB_FILENAME);
    if (tagId == 0) {
        db << "UPDATE schedule_scheduleitem SET name = ?, date = ?, done = ?, tag_id = NULL WHERE id = ? AND user_id = ?;"
            << name << date << done << scheduleItemId << userId;
    } else {
        db << "UPDATE schedule_scheduleitem SET name = ?, date = ?, done = ?, tag_id = ? WHERE id = ? AND user_id = ?;"
            << name << date << done << tagId << scheduleItemId << userId;
    }
}

void deleteScheduleItem(int userId, int scheduleItemId) {
    database db(DB_FILENAME);
    db << "DELETE FROM schedule_scheduleitem WHERE id = ? AND user_id = ?;"
        << scheduleItemId << userId;
}