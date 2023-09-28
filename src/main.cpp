#include <crow.h>
#include <main.h>

App<CORSMiddleware, AuthMiddleware> app;

int main() {
    setupUserRoutes();
    setupScheduleRoutes();
    setupTagRoutes();
    app.port(8080).multithreaded().run();
}