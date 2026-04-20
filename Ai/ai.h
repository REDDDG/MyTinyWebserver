#ifndef AI_HANDLER_H
#define AI_HANDLER_H

#include <string>

class AIHandler {
public:
    static void init(const std::string& api_key);
    static std::string chat(const std::string& message);
    static bool isAvailable() { return !api_key_.empty(); }

private:
    static std::string api_key_;
    static bool use_stream_;
};

#endif