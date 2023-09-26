#include <main.h>
using namespace sqlite;

string DB_FILENAME = "db.sqlite3";

User getUserByUsername(string username) {
    database db(DB_FILENAME);
    User user;
    db << "SELECT username, password FROM auth_user WHERE username = ?;" << username
        >> [&](string username, string password) {
            user.username = username;
            user.password = password;
        };
    return user;
}