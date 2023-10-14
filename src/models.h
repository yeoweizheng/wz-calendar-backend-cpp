#ifndef MODELS_H
#define MODELS_H
#include <string>
using namespace std;

struct User {
    int id;
    string username;
    string password;
};

struct ScheduleItem {
    int id;
    string name;
    string date;
    string time;
    bool done;
    int tagId;
};

struct Tag {
    int id;
    string name;
};

#endif