#ifndef APP_H
#define APP_H

#include <crow.h>
#include <string>
#include <middlewares.h>
#include <models.h>
using namespace std;
using namespace crow;

extern App<CORSMiddleware, AuthMiddleware> app;

// routes
void setupUserRoutes();
void setupScheduleRoutes();
void setupTagRoutes();

// utils
json::wvalue structToWValue(ScheduleItem scheduleItem);
json::wvalue structToWValue(Tag tag);

#endif
