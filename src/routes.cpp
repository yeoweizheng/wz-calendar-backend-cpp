#include <main.h>
#include <bcrypt/BCrypt.hpp>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <constants.h>

void setupUserRoutes() {
    CROW_ROUTE(app, "/token/")
    .methods(HTTPMethod::POST)([](const request& req){
        auto reqBody = json::load(req.body);
        crow::json::wvalue payload;
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
    ([](const request& req) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        crow::json::wvalue payload;
        vector<ScheduleItem> scheduleItems;
        scheduleItems = getScheduleItemsByUserIdAndDates(userId);
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
    });
}