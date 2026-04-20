#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "ai.h"
#include "httplib.h"
#include "../nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

std::string AIHandler::api_key_;
bool AIHandler::use_stream_ = false;

void AIHandler::init(const std::string& api_key) {
    api_key_ = api_key;
    std::cout << "AI Handler initialized" << std::endl;
}
std::string escape_json(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                o << c;
                break;
        }
    }
    return o.str();
}

std::string AIHandler::chat(const std::string& message) {
    if (api_key_.empty()) {
        return "AI服务未初始化，请检查API Key配置";
    }

    httplib::SSLClient cli("api.deepseek.com", 443);
    cli.enable_server_certificate_verification(false);

    json req;
    req["model"] = "deepseek-chat";
    req["stream"] = false;
    req["messages"] = {
            {{"role", "user"}, {"content", message}}
    };
    req["max_tokens"] = 5000;

    httplib::Headers headers = {
            {"Authorization", "Bearer " + api_key_},
            {"Content-Type", "application/json"}
    };

    auto res = cli.Post("/v1/chat/completions", headers, req.dump(), "application/json");

    if (res && res->status == 200) {
        try {
            json response = json::parse(res->body);
            return escape_json(response["choices"][0]["message"]["content"]);
        } catch (const std::exception& e) {
            return "JSON解析错误: " + std::string(e.what());
        }
    } else if (res) {
        return "API错误: " + std::to_string(res->status);
    } else {
        return "连接DeepSeek API失败";
    }
}