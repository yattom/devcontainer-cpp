#include "bad/trainroute/domain/data_store.h"

namespace bad::trainroute::domain {

std::string DataStore::lineData[2][2] = {
    {"中央線", "新宿,中野,吉祥寺,東京"},
    {"山手線", "新宿,上野,東京,品川"}
};

std::string DataStore::getStationList(const std::string& lineName) {
    for (int i = 0; i < 2; i++) {
        if (lineData[i][0] == lineName) {
            return lineData[i][1];
        }
    }
    return "";
}

std::vector<std::string> DataStore::getAllLines() {
    std::vector<std::string> lines;
    for (int i = 0; i < 2; i++) {
        lines.push_back(lineData[i][0]);
    }
    return lines;
}

std::string DataStore::getLineForStation(const std::string& station) {
    for (int i = 0; i < 2; i++) {
        std::string stations = lineData[i][1];
        if (stations.find(station) != std::string::npos) {
            return lineData[i][0];
        }
    }
    return "";
}

bool DataStore::isStationOnLine(const std::string& station, const std::string& line) {
    std::string stationList = getStationList(line);
    return stationList.find(station) != std::string::npos;
}

}  // namespace bad::trainroute::domain
