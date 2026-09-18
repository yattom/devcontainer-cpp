#ifndef BAD_TRAINROUTE_DOMAIN_TRAIN_SYSTEM_H
#define BAD_TRAINROUTE_DOMAIN_TRAIN_SYSTEM_H

#include <string>

namespace bad::trainroute::domain {

class TrainSystem {
public:
    static std::string getRouteInfo(const std::string& s1, const std::string& s2);

    static std::string findAllRoutes(const std::string& from, const std::string& to);

    static std::string getTimetableForStation(const std::string& station);

    static std::string showSchedule(const std::string& st);

    static bool isValidStation(const std::string& station);

    static bool stationExists(const char* s);

    static bool checkStation(const std::string& station);
};

}  // namespace bad::trainroute::domain

#endif  // BAD_TRAINROUTE_DOMAIN_TRAIN_SYSTEM_H
