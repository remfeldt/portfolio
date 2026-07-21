#pragma once

#include <string>
#include <unordered_map>
#include <functional>

class Router
{
public:

    using Handler =
        std::function<void(int)>;

    void addRoute(
        const std::string& path,
        Handler handler
    );

    Handler* findRoute(
        const std::string& path
    );

private:

    std::unordered_map<
        std::string,
        Handler
    > routes;
};