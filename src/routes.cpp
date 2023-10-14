#include <main.h>
#include <models.h>
#include <bcrypt/BCrypt.hpp>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <constants.h>
#include <exception>

void setupUserRoutes() {
    CROW_ROUTE(app, "/token/")
    .methods(HTTPMethod::POST, HTTPMethod::OPTIONS)([](const request& req){
        auto reqBody = json::load(req.body);
        json::wvalue payload;
        User user;
        try { user = getUser(reqBody["username"].s()); }
        catch (exception &e) { return response(401); }
        if (BCrypt::validatePassword(reqBody["password"].s(), user.password)) {
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
    CROW_ROUTE(app, "/update_password/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::POST, HTTPMethod::OPTIONS)([](const request& req){
        int userId = app.get_context<AuthMiddleware>(req).userId;
        auto reqBody = json::load(req.body);
        User user = getUser(userId);
        if (BCrypt::validatePassword(reqBody["currentPassword"].s(), user.password)) {
            string newPasswordHash = BCrypt::generateHash(reqBody["newPassword"].s());
            updateUser(userId, user.username, newPasswordHash);
            return response(200);
        } else {
            return response(400);
        }
    });
}

void setupScheduleRoutes() {
    CROW_ROUTE(app, "/schedule_items/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::POST, HTTPMethod::OPTIONS)([](const request& req) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        if (req.method == HTTPMethod::GET) {
            char* startDateCStr = req.url_params.get("start_date");
            char* endDateCStr = req.url_params.get("end_date");
            char* tagIdCStr = req.url_params.get("tag");
            char* searchCStr = req.url_params.get("search_str");
            vector<ScheduleItem> scheduleItems;
            if (searchCStr != nullptr) {
                string searchStr = string(searchCStr);
                scheduleItems = getScheduleItems(userId, searchStr);
            } 
            else if (startDateCStr == nullptr || endDateCStr == nullptr) { return response(400); }
            else {
                string startDate = string(startDateCStr);
                string endDate = string(endDateCStr);
                if (tagIdCStr != nullptr) {
                    string tagIdStr = string(tagIdCStr);
                    int tagId;
                    if (tagIdStr == "u") { 
                        tagId = -1; 
                    } else { 
                        try { tagId = stoi(tagIdStr); }
                        catch (exception &e) { return response(400); }
                    }
                    scheduleItems = getScheduleItems(userId, startDate, endDate, tagId);
                } else {
                    scheduleItems = getScheduleItems(userId, startDate, endDate);
                }
            }
            json::wvalue payload = json::wvalue::list();
            int i = 0;
            for (auto &scheduleItem : scheduleItems) {
                payload[i] = structToWValue(scheduleItem);
                i++;
            }
            return response(payload);
        } else {
            auto body = json::load(req.body);
            int tagId = body["tag"].t() == json::type::Null ? 0 : body["tag"].i();
            int scheduleItemId;
            try { scheduleItemId = insertScheduleItem(userId, body["name"].s(), body["date"].s(), body["time"].s(), body["done"].b(), tagId); }
            catch (exception &e) { return response(400); }
            auto payload = json::wvalue(body);
            payload["id"] = scheduleItemId;
            return response(201, payload);
        } 
    });
    CROW_ROUTE(app, "/schedule_items/<int>/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::PATCH, HTTPMethod::DELETE, HTTPMethod::OPTIONS)([](const request& req, int scheduleItemId) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        if (req.method == HTTPMethod::GET) {
            ScheduleItem scheduleItem;
            try {
                scheduleItem = getScheduleItem(userId, scheduleItemId);
            } catch (exception &e) {
                return response(404);
            }
            auto payload = structToWValue(scheduleItem);
            return response(payload);
        }
        else if (req.method == HTTPMethod::PATCH) {
            auto body = json::load(req.body);
            int tagId = body["tag"].t() == json::type::Null ? 0 : body["tag"].i();
            try {
                updateScheduleItem(userId, scheduleItemId, body["name"].s(), body["date"].s(), body["time"].s(), body["done"].b(), tagId);
            } catch (exception &e) {
                return response(404);
            }
            auto payload = json::wvalue(body);
            payload["id"] = scheduleItemId;
            return response(200, payload);
        } else {
            try {
                deleteScheduleItem(userId, scheduleItemId);
            } catch (exception &e) {
                return response(404);
            }
            return response(204);
        }
    });
}

void setupTagRoutes() {
    CROW_ROUTE(app, "/tags/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::POST, HTTPMethod::OPTIONS)([](const request& req) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        if (req.method == HTTPMethod::GET) {
            vector<Tag> tags = getTags(userId);
            json::wvalue payload = json::wvalue::list();
            int i = 0;
            for (auto &tag : tags) {
                payload[i] = structToWValue(tag);
                i++;
            }
            return response(payload);
        }
        else {
            auto body = json::load(req.body);
            int tagId = insertTag(userId, body["name"].s());
            auto payload = json::wvalue(body);
            payload["id"] = tagId;
            return response(201, payload);
        } 
    });
    CROW_ROUTE(app, "/tags/<int>/")
    .CROW_MIDDLEWARES(app, AuthMiddleware)
    .methods(HTTPMethod::GET, HTTPMethod::PATCH, HTTPMethod::DELETE, HTTPMethod::OPTIONS)([](const request& req, int tagId) {
        int userId = app.get_context<AuthMiddleware>(req).userId;
        if (req.method == HTTPMethod::GET) {
            Tag tag;
            try {
                tag = getTag(userId, tagId);
            } catch (exception &e) {
                return response(404);
            }
            auto payload = structToWValue(tag);
            return response(payload);
        }
        else if (req.method == HTTPMethod::PATCH) {
            auto body = json::load(req.body);
            try {
                updateTag(userId, tagId, body["name"].s());
            } catch (exception &e) {
                return response(404);
            }
            auto payload = json::wvalue(body);
            payload["id"] = tagId;
            return response(200, payload);
        } else {
            try {
                deleteTag(userId, tagId);
            } catch (exception &e) {
                return response(404);
            }
            return response(204);
        } 
    });
}