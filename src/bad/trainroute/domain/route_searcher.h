#ifndef BAD_TRAINROUTE_DOMAIN_ROUTE_SEARCHER_H
#define BAD_TRAINROUTE_DOMAIN_ROUTE_SEARCHER_H

#include <string>

namespace bad::trainroute::domain {

class RouteSearcher {
public:
    static std::string searchRoute(const std::string& from, const std::string& to);

    static std::string findRoute(const std::string& departure, const std::string& arrival);
};

}  // namespace bad::trainroute::domain

#endif  // BAD_TRAINROUTE_DOMAIN_ROUTE_SEARCHER_H
