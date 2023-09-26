#include <main.h>

void setupUserRoutes() {
    CROW_ROUTE(app, "/token/")
    .methods(HTTPMethod::POST)([](const request& req){
        auto reqBody = json::load(req.body);
        crow::json::wvalue payload;
        User user = getUserByUsername((string) reqBody["username"]);
        // for (auto& e : decoded.get_payload_json()) {
        //     cout << e.first << "=" << e.second << endl;
        // }
        if (BCrypt::validatePassword((string) reqBody["password"], user.password)) {
            payload["token"] = jwt::create().set_type("JWT").set_payload_claim("username", jwt::claim(string(user.username))).sign(jwt::algorithm::hs256{"secret"});  // TODO: set secret in config file
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
        cout << app.get_context<AuthMiddleware>(req).username;
        return "Hello";
    });
}