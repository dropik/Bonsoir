#pragma once

#include "bonsoir_action.h"

namespace bonsoir_windows {
  class BonsoirDiscovery : public BonsoirAction {
   public:
    std::string type;
    std::list<std::shared_ptr<BonsoirService>> services;
    std::map<std::shared_ptr<BonsoirService>, PDNS_SERVICE_CANCEL> resolvingServices = std::map<std::shared_ptr<BonsoirService>, PDNS_SERVICE_CANCEL>{};
    std::mutex resolvingServicesCleanupMutex;
    // Guards `services` and every service in it, which the DNS-SD callbacks change on the thread
    // pool while the platform thread reads them. Always taken before resolvingServicesCleanupMutex.
    std::recursive_mutex servicesMutex;

    BonsoirDiscovery(
      int _id,
      bool _printLogs,
      flutter::BinaryMessenger *_binaryMessenger,
      std::string type
    );

    ~BonsoirDiscovery();

    void start() override;

    void stop() override;

    std::shared_ptr<BonsoirService> findService(std::string serviceName, std::string serviceType);

    void resolveService(std::string serviceName, std::string serviceType);

    void dispose() override;
  };

  void browseCallback(DWORD status, PVOID context, PDNS_RECORD dnsRecord);

  void resolveCallback(DWORD status, PVOID context, PDNS_SERVICE_INSTANCE serviceInstance);
}  // namespace bonsoir_windows
