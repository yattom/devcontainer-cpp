#include "bad/trainroute/domain/transfer_handler.h"

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

std::vector<std::string> TransferHandler::findTransferStations(const std::string& from,
                                                               const std::string& to) {
    std::string result[10];
    int count = 0;

    std::vector<std::string> lines = DataStore::getAllLines();

    for (int i = 0; i < static_cast<int>(lines.size()); i++) {
        if (DataStore::isStationOnLine(from, lines[i])) {
            for (int j = 0; j < static_cast<int>(lines.size()); j++) {
                if (lines[i] != lines[j] && DataStore::isStationOnLine(to, lines[j])) {
                    std::string stationsOnLineI = DataStore::getStationList(lines[i]);
                    std::vector<std::string> stArray = split(stationsOnLineI, ',');

                    for (int k = 0; k < static_cast<int>(stArray.size()); k++) {
                        if (DataStore::isStationOnLine(stArray[k], lines[j])) {
                            if (count < 10) {
                                result[count] = stArray[k];
                                count++;
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<std::string> finalResult;
    for (int i = 0; i < count; i++) {
        finalResult.push_back(result[i]);
    }

    return finalResult;
}

std::string TransferHandler::buildRoute(const std::string& from, const std::string& via,
                                        const std::string& to) {
    std::string line1 = DataStore::getLineForStation(from);
    std::string line2 = DataStore::getLineForStation(to);

    if (line1 == "" || line2 == "") {
        return "ルートが見つかりません";
    }

    std::string route1 = from + "で" + line1 + "に乗車して、" + via + "で下車します。";
    std::string route2 = via + "で" + line2 + "に乗車して、" + to + "で下車します。";

    return route1 + " " + route2;
}

std::vector<std::string> TransferHandler::getRoutes(const std::string& start,
                                                    const std::string& end) {
    std::vector<std::string> transfers = findTransferStations(start, end);
    std::vector<std::string> routes;

    for (int i = 0; i < static_cast<int>(transfers.size()); i++) {
        routes.push_back(buildRoute(start, transfers[i], end));
    }

    return routes;
}

}  // namespace bad::trainroute::domain
