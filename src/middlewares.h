
#ifndef MIDDLEWARES_H
#define MIDDLEWARES_H

#include <iostream>
#include <crow.h>
using namespace std;
using namespace crow;

struct AuthMiddleware : ILocalMiddleware {
    struct context {
        string username;
    };
    void before_handle(request& req, response& res, context& ctx) {
        ctx.username = "weizheng";
    };
    void after_handle(request& req, response& res, context& ctx) {};
};

#endif