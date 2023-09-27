#include <main.h>
#include <bcrypt/BCrypt.hpp>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <constants.h>

void setupUserRoutes() {
    CROW_ROUTE(app, "/token/")
    .methods(HTTPMethod::POST)([](const request& req){
        auto reqBody = json::load(req.body);
        json::wvalue payload;
        User user = getUserByUsername((string) reqBody["username"]);
        if (BCrypt::validatePassword((string) reqBody["password"], user.password)) {
            time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            stringstream nowSS;
            nowSS << put_time(localtime(&now), "%FT%T%z");
            payload["token"] = jwt::create().set_type("JWT")
                .set_payload_claim("userId", jwt::claim(string(to_string(user.id))))
                .set_payload_claim("created", jwt::claim(string(nowSS.str())))
                .sign(jwt::algorithm::hs256{JWT_SECRET});
            return response(payload);
        } else {
            return response(401);
        }
    });
}

void setupScheduleRoutes() {
    CROW_ROUTE(app, "/schedule_items/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::POST)([](const request& req) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        if (req.method == HTTPMethod::GET) {
            char* startDateCStr = req.url_params.get("start_date");
            char* endDateCStr = req.url_params.get("end_date");
            char* tagIdCStr = req.url_params.get("tag");
            char* searchCStr = req.url_params.get("search_str");
            if (startDateCStr == nullptr || endDateCStr == nullptr) { return response(400); }
            string startDate = string(startDateCStr);
            string endDate = string(endDateCStr);
            vector<ScheduleItem> scheduleItems;
            if (tagIdCStr != nullptr) {
                string tagIdStr = string(tagIdCStr);
                int tagId;
                if (tagIdStr == "u") { tagId = -1; } else { tagId = stoi(tagIdStr); }
                scheduleItems = getScheduleItemsByUserIdAndDates(userId, startDate, endDate, tagId);
            } else {
                scheduleItems = getScheduleItemsByUserIdAndDates(userId, startDate, endDate);
            }
            json::wvalue payload = json::wvalue::list();
            int i = 0;
            for (auto &scheduleItem : scheduleItems) {
                payload[i]["id"] = scheduleItem.id;
                payload[i]["name"] = scheduleItem.name;
                payload[i]["date"] = scheduleItem.date;
                payload[i]["done"] = scheduleItem.done;
                if (scheduleItem.tagId == 0) { payload[i]["tagId"] = NULL; } else { payload[i]["tagId"] = scheduleItem.tagId; }
                i++;
            }
            return response(payload);
        }
        else {
            auto body = json::load(req.body);
            stringstream tagIdSS;
            tagIdSS << body["tag"];
            if (tagIdSS.str() == "null") {
                insertScheduleItem(userId, (string) body["name"], (string) body["date"], (bool) body["done"]);
            } else {
                int tagId = stoi(tagIdSS.str());
                insertScheduleItem(userId, (string) body["name"], (string) body["date"], (bool) body["done"], tagId);
            }
            return response(201);
        }
    });
}