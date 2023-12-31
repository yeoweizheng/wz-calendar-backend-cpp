#include <db.h>
#include <models.h>
#include <exception>

void checkUserExists(string username) {
    *db << "SELECT COUNT(id) FROM user WHERE username = ?;"
        << username
        >> [](int count) {
            if (count != 1) { throw runtime_error("Not found"); }
        };
}

User getUser(string username) {
    checkUserExists(username);
    User user;
    *db << "SELECT id, username, password FROM user WHERE username = ?;" << username
        >> [&](int userId, string username, string password) {
            user.id = userId;
            user.username = username;
            user.password = password;
        };
    return user;
}

User getUser(int userId) {
    User user;
    *db << "SELECT id, username, password FROM user WHERE id = ?;" << userId
        >> [&](int userId, string username, string password) {
            user.id = userId;
            user.username = username;
            user.password = password;
        };
    return user;
}

void updateUser(int userId, string username, string password) {
    *db << "UPDATE user SET username = ?, password = ? WHERE id = ?;"
        << username << password << userId;
}

vector<ScheduleItem> getScheduleItems(int userId, string startDate, string endDate, int tagId) {
    vector<ScheduleItem> scheduleItems;
    auto populateScheduleItems = [&](int id, string name, string date, string time, bool done, int tagId) {
        ScheduleItem scheduleItem = { id, name, date, time, done, tagId };
        scheduleItems.push_back(scheduleItem);
    };
    if (tagId == 0) {
        *db << "SELECT id, name, date, time, done, tag_id FROM scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?) ORDER BY time ASC NULLS LAST, id ASC;"
            << userId << startDate << endDate
            >> populateScheduleItems;
    } else if (tagId == -1) {
            *db << "SELECT id, name, date, time, done, tag_id FROM scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?) AND tag_id IS NULL ORDER BY time ASC NULLS LAST, id ASC;"
                << userId << startDate << endDate
                >> populateScheduleItems;
    } else {
        *db << "SELECT id, name, date, time, done, tag_id FROM scheduleitem WHERE user_id = ? AND date >= date(?) AND date <= date(?) AND tag_id = ? ORDER BY time ASC NULLS LAST, id ASC;"
            << userId << startDate << endDate << tagId
            >> populateScheduleItems;
    }
    return scheduleItems;
}

vector<ScheduleItem> getScheduleItems(int userId, string searchStr) {
    vector<ScheduleItem> scheduleItems;
    searchStr = "%" + searchStr + "%";
    *db << "SELECT id, name, date, time, done, tag_id FROM scheduleitem WHERE user_id = ? AND done = false AND name LIKE ? ORDER BY date ASC, time ASC;"
        << userId << searchStr
        >> [&](int id, string name, string date, string time, bool done, int tagId) {
            ScheduleItem scheduleItem = { id, name, date, time, done, tagId };
            scheduleItems.push_back(scheduleItem);
        };
    return scheduleItems;
}

int insertScheduleItem(int userId, string name, string date, string time, bool done, int tagId) {
    if (tagId != 0) checkTagExists(userId, tagId);
    if (tagId == 0) {
        if (time == "") {
            *db << "INSERT INTO scheduleitem (user_id, name, date, done) VALUES (?, ?, ?, ?);"
                << userId << name << date << done;
        } else {
            * db << "INSERT INTO scheduleitem (user_id, name, date, time, done) VALUES (?, ?, ?, ?, ?);"
                << userId << name << date << time << done;
        }
    } else {
        if (time == "") {
            *db << "INSERT INTO scheduleitem (user_id, name, date, done, tag_id) VALUES (?, ?, ?, ?, ?);"
                << userId << name << date << done << tagId;
        } else {
            *db << "INSERT INTO scheduleitem (user_id, name, date, time, done, tag_id) VALUES (?, ?, ?, ?, ?, ?);"
                << userId << name << date << time << done << tagId;
        }
    }
    return db->last_insert_rowid();
}

void checkScheduleItemExists(int userId, int scheduleItemId) {
    *db << "SELECT COUNT(id) FROM scheduleitem WHERE id = ? AND user_id = ?;"
        << scheduleItemId << userId
        >> [](int count) {
            if (count != 1) { throw runtime_error("Not found"); }
        };
}

ScheduleItem getScheduleItem(int userId, int scheduleItemId) {
    checkScheduleItemExists(userId, scheduleItemId);
    ScheduleItem scheduleItem;
    *db << "SELECT id, name, date, time, done, tag_id FROM scheduleitem WHERE user_id = ? AND id = ?;"
        << userId << scheduleItemId
        >> [&](int id, string name, string date, string time, bool done, int tagId) {
            scheduleItem = { id, name, date, time, done, tagId };
        };
    return scheduleItem;
}

void updateScheduleItem(int userId, int scheduleItemId, string name, string date, string time, bool done, int tagId) {
    checkScheduleItemExists(userId, scheduleItemId);
    if (tagId == 0) {
        if (time == "") {
            *db << "UPDATE scheduleitem SET name = ?, date = ?, time = NULL, done = ?, tag_id = NULL WHERE id = ? AND user_id = ?;"
                << name << date << done << scheduleItemId << userId;
        } else {
            *db << "UPDATE scheduleitem SET name = ?, date = ?, time = ?, done = ?, tag_id = NULL WHERE id = ? AND user_id = ?;"
                << name << date << time << done << scheduleItemId << userId;
        }
    } else {
        if (time == "") {
            *db << "UPDATE scheduleitem SET name = ?, date = ?, time = NULL, done = ?, tag_id = ? WHERE id = ? AND user_id = ?;"
                << name << date << done << tagId << scheduleItemId << userId;
        } else {
            *db << "UPDATE scheduleitem SET name = ?, date = ?, time = ?, done = ?, tag_id = ? WHERE id = ? AND user_id = ?;"
                << name << date << time << done << tagId << scheduleItemId << userId;
        }
    }
}

void deleteScheduleItem(int userId, int scheduleItemId) {
    checkScheduleItemExists(userId, scheduleItemId);
    *db << "DELETE FROM scheduleitem WHERE id = ? AND user_id = ?;"
        << scheduleItemId << userId;
}

vector<Tag> getTags(int userId) {
    vector<Tag> tags;
    *db << "SELECT id, name FROM tag WHERE user_id = ?;"
        << userId
        >> [&](int id, string name) {
            Tag tag = { id, name };
            tags.push_back(tag);
        };
    return tags;
}

int insertTag(int userId, string name) {
    *db << "INSERT INTO tag (user_id, name) VALUES (?, ?);"
        << userId << name;
    return db->last_insert_rowid();
}

void checkTagExists(int userId, int tagId) {
    *db << "SELECT COUNT(id) FROM tag WHERE id = ? AND user_id = ?;"
        << tagId << userId
        >> [](int count) {
            if (count != 1) { throw runtime_error("Not found"); }
        };
}

Tag getTag(int userId, int tagId) {
    checkTagExists(userId, tagId);
    Tag tag;
    *db << "SELECT id, name FROM tag WHERE user_id = ? AND id = ?;"
        << userId << tagId
        >> [&](int id, string name) {
            tag = { id, name };
        };
    return tag;
}

void updateTag(int userId, int tagId, string name) {
    checkTagExists(userId, tagId);
    *db << "UPDATE tag SET name = ? WHERE id = ? AND user_id = ?;"
        << name << tagId << userId;
}

void deleteTag(int userId, int tagId) {
    checkTagExists(userId, tagId);
    *db << "UPDATE scheduleitem SET tag_id = NULL WHERE tag_id = ? AND user_id = ?;"
        << tagId << userId;
    *db << "DELETE FROM tag WHERE id = ? AND user_id = ?;"
        << tagId << userId;
}

string getJWTSecret() {
    string secret;
    *db << "SELECT value FROM keyvalue WHERE key = \"JWT_SECRET\";"
        >> [&](string s) {
            secret = s;
        };
    return secret;
}