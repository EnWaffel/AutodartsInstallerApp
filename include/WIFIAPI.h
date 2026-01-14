#pragma once

#include "CommandAPI.h"
#include "panel/ConsolePanel.h"

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
    WIFIAPI(CommandAPI& cmdAPI, ConsolePanel* console);

    WIFIError GetAvailableNetworks(std::vector<std::string>& networks);
    WIFIError ConnectViaWPS(const std::string& ssid);
    WIFIError ConnectNormally(const std::string& ssid, const std::string& password);
protected:
    CommandAPI& cmdAPI;
    ConsolePanel* console;
};