#ifndef BAD_TRAINROUTE_DOMAIN_TRANSFER_HANDLER_H
#define BAD_TRAINROUTE_DOMAIN_TRANSFER_HANDLER_H

#include <string>
#include <vector>

namespace bad::trainroute::domain {

class TransferHandler {
public:
    static std::vector<std::string> findTransferStations(const std::string& from,
                                                         const std::string& to);

    static std::string buildRoute(const std::string& from, const std::string& via,
                                  const std::string& to);

    static std::vector<std::string> getRoutes(const std::string& start, const std::string& end);
};

}  // namespace bad::trainroute::domain

#endif  // BAD_TRAINROUTE_DOMAIN_TRANSFER_HANDLER_H
