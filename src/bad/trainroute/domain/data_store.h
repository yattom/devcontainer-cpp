#ifndef BAD_TRAINROUTE_DOMAIN_DATA_STORE_H
#define BAD_TRAINROUTE_DOMAIN_DATA_STORE_H

#include <string>
#include <vector>

namespace bad::trainroute::domain {

class DataStore {
public:
    static std::string lineData[2][2];

    static std::string getStationList(const std::string& lineName);

    static std::vector<std::string> getAllLines();

    static std::string getLineForStation(const std::string& station);

    static bool isStationOnLine(const std::string& station, const std::string& line);
};

}  // namespace bad::trainroute::domain

#endif  // BAD_TRAINROUTE_DOMAIN_DATA_STORE_H
