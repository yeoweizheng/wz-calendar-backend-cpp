#include <crow.h>
#include <models.h>
#include <string>
using namespace std;
using namespace crow;

json::wvalue structToWValue(ScheduleItem scheduleItem) {
    json::wvalue payload;
    payload["id"] = scheduleItem.id;
    payload["name"] = scheduleItem.name;
    payload["date"] = scheduleItem.date;
    payload["time"] = scheduleItem.time;
    payload["notes"] = scheduleItem.notes;
    payload["done"] = scheduleItem.done;
    if (scheduleItem.tagId == 0) { payload["tag"] = NULL; } else { payload["tag"] = scheduleItem.tagId; }
    return payload;
}

json::wvalue structToWValue(Tag tag) {
    json::wvalue payload;
    payload["id"] = tag.id;
    payload["name"] = tag.name;
    return payload;
}
