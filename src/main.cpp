#include <crow.h>
#include <main.h>

App<AuthMiddleware> app;

int main() {
    CROW_ROUTE(app, "/")([](){
        return "Hello world!";
    });
    setupUserRoutes();
    setupScheduleRoutes();
    app.port(8080).multithreaded().run();
}