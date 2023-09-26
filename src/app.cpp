#include "crow.h"
#include "app.h"

crow::SimpleApp app;

int main() {
    CROW_ROUTE(app, "/")([](){
        return "Hello world!";
    });
    setup_user_routes();
    app.port(8080).multithreaded().run();
}