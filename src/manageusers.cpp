#include <iostream>
#include <string>
#include <sqlite_modern_cpp.h>
#include <unistd.h>
#include <bcrypt/BCrypt.hpp>

using namespace std;
using namespace sqlite;

unique_ptr<database> db;
void initDB();
void addUserMenu();
void editUserMenu();
void updateSecretMenu();

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: manageusers <database_file>" << endl;
        exit(EXIT_SUCCESS);
    }
    string db_filename = string(argv[1]);
    db = make_unique<database>(db_filename);
    string input;
    while(1) {
        cout << "\n[Main Menu] (Ctrl-C to exit)" << endl;
        cout << "i> Initialize database" << endl;
        cout << "a> Add user" << endl;
        cout << "e> Edit user" << endl;
        cout << "s> Update JWT secret" << endl;
        cout << "Select an option: ";
        cin >> input;
        if (input == "i") {
            initDB();
        } else if (input == "a") {
            addUserMenu();
        } else if (input == "e") {
            editUserMenu();
        } else if (input == "s") {
            updateSecretMenu();
        }
    }
    return 0;
}

void initDB() {
    *db << "CREATE TABLE IF NOT EXISTS user (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"username\" varchar(150) NOT NULL UNIQUE, \"password\" varchar(128) NOT NULL);";
    *db << "CREATE TABLE IF NOT EXISTS scheduleitem (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"name\" varchar(512) NOT NULL, \"date\" date NOT NULL, \"time\" time NULL, \"user_id\" integer NOT NULL REFERENCES \"user\" (\"id\") DEFERRABLE INITIALLY DEFERRED, \"done\" bool NOT NULL, \"tag_id\" bigint NULL REFERENCES \"tag\" (\"id\") DEFERRABLE INITIALLY DEFERRED);";
    *db << "CREATE TABLE IF NOT EXISTS \"tag\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"name\" varchar(100) NOT NULL, \"user_id\" integer NOT NULL REFERENCES \"user\" (\"id\") DEFERRABLE INITIALLY DEFERRED);";
    *db << "CREATE TABLE IF NOT EXISTS \"keyvalue\" (\"key\" varchar(100) NOT NULL, \"value\" varchar(100) NOT NULL);";
    *db << "CREATE INDEX IF NOT EXISTS \"scheduleitem_tag_id\" ON \"scheduleitem\" (\"tag_id\");";
    *db << "CREATE INDEX IF NOT EXISTS \"scheduleitem_user_id\" ON \"scheduleitem\" (\"user_id\");";
    *db << "CREATE INDEX IF NOT EXISTS \"tag_user_id\" ON \"tag\" (\"user_id\");";
    *db << "DELETE FROM keyvalue WHERE key = \"JWT_SECRET\";";
    *db << "INSERT INTO keyvalue (key, value) VALUES (\"JWT_SECRET\", \"secret\")";
    cout << "Database initialised successfully. Please remember to update JWT secret." << endl;
}

bool userExists(string username) {
    int userCount;
    *db << "SELECT COUNT(id) FROM user WHERE username = ?;" << username >> [&](int c) { userCount = c; };
    if (userCount > 0) {
        return true;
    } else {
        return false;
    }
}

void addUserMenu() {
    string username;
    while(1) {
        cout << "Username: ";
        cin >> username;
        if (userExists(username)) { cout << "Username already taken." << endl; continue;}
        char *passwordCStr = getpass("Password: ");
        string password = string(passwordCStr);
        string passwordHash = BCrypt::generateHash(password);
        *db << "INSERT INTO user (username, password) VALUES (?, ?);" << username << passwordHash;
        cout << "User added successfully." << endl;
        break;
    }
}

void editUserMenu() {
    while(1) {
        string userIDStr;
        cout << "\n[User List]" << endl;
        *db << "SELECT id, username FROM user ORDER BY id ASC;"
        >> [&](int id, string username) { cout << id << " " << username << endl; };
        cout << "m> Return to main menu" << endl;
        cout << "Select user ID: ";
        cin >> userIDStr;
        if (userIDStr == "m") return;
        int userID;
        try { userID = stoi(userIDStr); } catch (...) { cout << "Invalid input" << endl; continue; }
        int userCount;
        *db << "SELECT COUNT(id) FROM user WHERE id = ?;" << userID >> [&](int c) {userCount = c;};
        if (userCount == 0) { cout << "Invalid user ID selected." << endl; continue; }
        while(1) {
            string choice;
            cout << "\n[User Actions] (ID: " << userID << ")" << endl;
            cout << "u> Change username" << endl;
            cout << "p> Change password" << endl;
            cout << "d> Delete user" << endl;
            cout << "m> Back to user list" << endl;
            cout << "Select an option: ";
            cin >> choice;
            if (choice == "u") {
                while(1) {
                    string username;
                    cout << "Enter new username: ";
                    cin >> username;
                    if (userExists(username)) { cout << "Username already taken." << endl; continue;}
                    *db << "UPDATE user SET username = ? WHERE id = ?" << username << userID;
                    cout << "Username updated." << endl;
                    break;
                }
                break;
            } else if (choice == "p") {
                char *passwordCStr = getpass("Password: ");
                string password = string(passwordCStr);
                string passwordHash = BCrypt::generateHash(password);
                *db << "UPDATE user SET password = ? WHERE id = ?;" << passwordHash << userID;
                cout << "Password updated." << endl;
                break;
            } else if (choice == "d") {
                string confirmation;
                cout << "Are you sure? (type YES to confirm): ";
                cin >> confirmation;
                if (confirmation == "YES") {
                    *db << "DELETE FROM scheduleitem WHERE user_id = ?;" << userID;
                    *db << "DELETE FROM tag WHERE user_id = ?;" << userID;
                    *db << "DELETE FROM user WHERE id = ?;" << userID;
                    cout << "User deleted." << endl;
                }
                break;
            } else if (choice == "m") break;
        }
    }
}

void updateSecretMenu() {
    char *secretCStr = getpass("Enter new JWT secret: ");
    string secret = string(secretCStr);
    *db << "UPDATE keyvalue SET value = ? WHERE key = \"JWT_SECRET\"" << secret;
    cout << "JWT secret updated." << endl;
}