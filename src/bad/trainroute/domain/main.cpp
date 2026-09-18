#include <iostream>
#include <string>

#include "bad/trainroute/domain/train_system.h"

using bad::trainroute::domain::TrainSystem;

int main() {
    std::cout << "==========" << std::endl;
    std::cout << "ルート検索: 新宿から吉祥寺" << std::endl;
    std::string route = TrainSystem::getRouteInfo("新宿", "吉祥寺");
    std::cout << route << std::endl;

    std::cout << "==========" << std::endl;
    std::cout << "時刻表取得: 新宿" << std::endl;
    std::string tt = TrainSystem::getTimetableForStation("新宿");
    std::cout << tt << std::endl;

    std::cout << "==========" << std::endl;
    std::cout << "駅存在確認: 中野" << std::endl;
    bool exists = TrainSystem::isValidStation("中野");
    std::cout << "中野 exists: " << (exists ? "true" : "false") << std::endl;

    std::cout << "==========" << std::endl;
    std::cout << "乗り換え経由ルート: 品川から中野" << std::endl;
    std::string transferRoute = TrainSystem::findAllRoutes("品川", "中野");
    std::cout << transferRoute << std::endl;

    return 0;
}
