#include "bad/trainroute/domain/train_system.h"

#include <vector>

#include "bad/trainroute/domain/route_searcher.h"
#include "bad/trainroute/domain/transfer_handler.h"

namespace bad::trainroute::domain {

std::string TrainSystem::getRouteInfo(const std::string& s1, const std::string& s2) {
    return RouteSearcher::searchRoute(s1, s2);
}

std::string TrainSystem::findAllRoutes(const std::string& from, const std::string& to) {
    std::string directRoute = RouteSearcher::searchRoute(from, to);

    if (directRoute != "駅が見つかりません" && directRoute != "不正なルートです") {
        return directRoute;
    }

    std::vector<std::string> transferRoutes = TransferHandler::getRoutes(from, to);

    if (transferRoutes.size() == 0) {
        return "ルートが見つかりません";
    }

    std::string result = "";
    for (int i = 0; i < static_cast<int>(transferRoutes.size()); i++) {
        result = result + std::to_string(i + 1) + ". " + transferRoutes[i] + "\n";
    }

    return result;
}

std::string TrainSystem::getTimetableForStation(const std::string& station) {
    if (station == "新宿") {
        std::string x = "新宿駅の時刻表:\n";
        x = x + "08:00 中央線吉祥寺行き\n";
        x = x + "08:15 中央線吉祥寺行き\n";
        x = x + "08:30 中央線吉祥寺行き\n";
        x = x + "08:45 中央線吉祥寺行き\n";
        return x;
    } else if (station == "中野") {
        std::string y = "中野駅の時刻表:\n";
        y = y + "08:05 中央線吉祥寺行き\n";
        y = y + "08:20 中央線吉祥寺行き\n";
        y = y + "08:35 中央線吉祥寺行き\n";
        y = y + "08:50 中央線吉祥寺行き\n";
        return y;
    } else if (station == "吉祥寺") {
        std::string z = "吉祥寺駅の時刻表:\n";
        z = z + "08:10 中央線新宿行き\n";
        z = z + "08:25 中央線新宿行き\n";
        z = z + "08:40 中央線新宿行き\n";
        z = z + "08:55 中央線新宿行き\n";
        return z;
    } else if (station == "品川") {
        std::string timetable = "品川駅の時刻表:\n";
        timetable = timetable + "08:00 山手線上野行き\n";
        timetable = timetable + "08:10 山手線上野行き\n";
        timetable = timetable + "08:20 山手線上野行き\n";
        timetable = timetable + "08:30 山手線上野行き\n";
        return timetable;
    } else if (station == "東京") {
        std::string t = "東京駅の時刻表:\n";
        t = t + "08:05 山手線新宿行き\n";
        t = t + "08:15 山手線新宿行き\n";
        t = t + "08:25 山手線新宿行き\n";
        t = t + "08:35 山手線新宿行き\n";
        return t;
    }
    return "不明な駅";
}

std::string TrainSystem::showSchedule(const std::string& st) {
    if (st == "新宿") {
        return "新宿駅の時刻表:\n08:00\n08:15\n08:30\n08:45";
    } else if (st == "中野") {
        return "中野駅の時刻表:\n08:05\n08:20\n08:35\n08:50";
    } else if (st == "吉祥寺") {
        return "吉祥寺駅の時刻表:\n08:10\n08:25\n08:40\n08:55";
    } else if (st == "品川") {
        return "品川駅の時刻表:\n08:00\n08:10\n08:20\n08:30";
    } else if (st == "東京") {
        return "東京駅の時刻表:\n08:05\n08:15\n08:25\n08:35";
    }
    return "駅なし";
}

bool TrainSystem::isValidStation(const std::string& station) {
    std::string validStations[3] = {"新宿", "中野", "吉祥寺"};
    for (int i = 0; i < 3; i++) {
        if (validStations[i] == station) {
            return true;
        }
    }
    return false;
}

bool TrainSystem::stationExists(const char* s) {
    const char* arr[3] = {"新宿", "中野", "吉祥寺"};
    for (int j = 0; j < 3; j++) {
        if (arr[j] == s) {
            return true;
        }
    }
    return false;
}

bool TrainSystem::checkStation(const std::string& station) {
    if (station == "新宿" || station == "中野" || station == "吉祥寺" ||
        station == "品川" || station == "東京" || station == "上野") {
        return true;
    }
    return false;
}

}  // namespace bad::trainroute::domain
