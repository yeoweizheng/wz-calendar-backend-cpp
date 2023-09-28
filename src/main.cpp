#include <crow.h>
#include <main.h>

App<CORSMiddleware, AuthMiddleware> app;

int main() {
    setupUserRoutes();
    setupScheduleRoutes();
    setupTagRoutes();
    app.port(8002).concurrency(4).run();
}