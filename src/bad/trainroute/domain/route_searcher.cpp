#include "bad/trainroute/domain/route_searcher.h"

#include <vector>

#include "bad/trainroute/domain/data_store.h"

namespace bad::trainroute::domain {

static std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> parts;
    std::string current = "";
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == sep) {
            parts.push_back(current);
            current = "";
        } else {
            current = current + s[i];
        }
    }
    parts.push_back(current);
    return parts;
}

std::string RouteSearcher::searchRoute(const std::string& from, const std::string& to) {
    std::string lineName = "中央線";

    std::string stStr = DataStore::getStationList(lineName);
    std::vector<std::string> stations = split(stStr, ',');

    int a = -1;
    int b = -1;

    for (int i = 0; i < static_cast<int>(stations.size()); i++) {
        if (stations[i] == from) {
            a = i;
        }
        if (stations[i] == to) {
            b = i;
        }
    }

    if (a == -1 || b == -1) {
        return "駅が見つかりません";
    }

    if (a >= b) {
        return "不正なルートです";
    }

    std::string result = from + "で" + lineName + "に乗車して、" + to + "で下車します。";

    return result;
}

std::string RouteSearcher::findRoute(const std::string& departure, const std::string& arrival) {
    std::string line = "中央線";

    std::string sStr = DataStore::getStationList(line);
    std::vector<std::string> stationArray = split(sStr, ',');

    int idx1 = -1;
    int idx2 = -1;

    for (int i = 0; i < static_cast<int>(stationArray.size()); i++) {
        if (stationArray[i] == departure) {
            idx1 = i;
        }
        if (stationArray[i] == arrival) {
            idx2 = i;
        }
    }

    if (idx1 == -1 || idx2 == -1) {
        return "駅が見つかりません";
    }

    if (idx1 >= idx2) {
        return "不正なルートです";
    }

    std::string ans = departure + "で" + line + "に乗車して、" + arrival + "で下車します。";

    return ans;
}

}  // namespace bad::trainroute::domain
