#pragma once

#include <cstdint>
#include <vector>
#include <string>

#define WIFIAPI_SUCCESS 0
#define WIFIAPI_ERROR_INCORRECT_PASSWORD 1
#define WIFIAPI_ERROR_NOT_REACHABLE 2
#define WIFIAPI_ERROR_OTHER 999

using WIFIError = uint16_t;

class WIFIAPI {
public:
    WIFIAPI() = default;
    virtual ~WIFIAPI() = default;

    virtual std::vector<std::string> GetAvailableNetworks() = 0;
    virtual WIFIError ConnectViaWPS() = 0;
    virtual WIFIError ConnectNormally(const std::string& ssid, const std::string& password) = 0;
};

extern WIFIAPI* _WIFIAPI;