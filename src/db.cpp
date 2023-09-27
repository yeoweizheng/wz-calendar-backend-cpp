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

vector<ScheduleItem> getScheduleItemsByUserIdAndDates(int userId) {
    database db(DB_FILENAME);
    vector<ScheduleItem> scheduleItems;
    db << "SELECT id, name, date, done, tag_id FROM schedule_scheduleitem WHERE user_id = ?;" << userId
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