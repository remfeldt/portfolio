#include "router.h"

void Router::addRoute(
    const std::string& path,
    Handler handler
)
{
    routes[path] = handler;
}

Router::Handler* Router::findRoute(
    const std::string& path
)
{
    auto route =
        routes.find(path);

    if (route == routes.end())
    {
        return nullptr;
    }

    return &route->second;
}