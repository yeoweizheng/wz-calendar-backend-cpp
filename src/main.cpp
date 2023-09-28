#include <crow.h>
#include <main.h>
#include <fstream>
#include <exception>

App<CORSMiddleware, AuthMiddleware> app;
unique_ptr<database> db;

int main() {
    ifstream ifile;
    ifile.open(DB_FILENAME);
    if (!ifile) {
        cerr << "ERROR: Database file not found" << endl;
        exit(EXIT_FAILURE);
    }
    db = make_unique<database>(DB_FILENAME);
    setupUserRoutes();
    setupScheduleRoutes();
    setupTagRoutes();
    app.port(8002).concurrency(4).run();
}