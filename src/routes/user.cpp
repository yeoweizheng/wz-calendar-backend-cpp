#include "crow.h"
#include "app.h"

void setup_user_routes() {
    CROW_ROUTE(app, "/login")([](){
        return "Login";
    });
}