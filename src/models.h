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
    bool done;
    int tagId;
};

#endif