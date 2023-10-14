
#ifndef MIDDLEWARES_H
#define MIDDLEWARES_H
#include <iostream>
#include <crow.h>
#include <boost/algorithm/string/trim.hpp>
#include <jwt-cpp/jwt.h>
#include <constants.h>
#include <db.h>

using namespace std;
using namespace crow;
using namespace boost::algorithm;

struct CORSMiddleware {
    struct context {};
    void before_handle(request& req, response& res, context& ctx) { }
    void after_handle(request& req, response& res, context& ctx) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        if (req.method == HTTPMethod::OPTIONS) {
            res.code = 200;
            res.end();
        }
    };
};

struct AuthMiddleware : ILocalMiddleware {
    struct context {
        int userId;
    };
    void before_handle(request& req, response& res, context& ctx) {
        string token = trim_left_copy_if(req.get_header_value("Authorization"), is_any_of("JWT "));
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256(getJWTSecret()));
            verifier.verify(decoded);
            for (auto& e : decoded.get_payload_json()) {
                stringstream keySS;
                stringstream valueSS;
                keySS << e.first;
                valueSS << e.second;
                if (keySS.str() == "userId") {
                    ctx.userId = stoi(trim_copy_if(valueSS.str(), is_any_of("\"")));
                    break;
                }
            }
        } catch (...) {
            res.code = 401;
            res.end();
        }
    };
    void after_handle(request& req, response& res, context& ctx) {};
};

#endif