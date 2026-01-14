#ifndef _WIN32
#include "WIFIAPI.h"

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

class Windows_WIFIAPI : public WIFIAPI {
public:
    std::vector<std::string> GetAvailableNetworks() override {
        std::cout << "[WIFI] Scanning for networks...\n";

        // Simulate scan delay
        std::this_thread::sleep_for(std::chrono::seconds(1));

        return {
            "Home_WiFi",
            "Office_Network",
            "CoffeeShop_FreeWiFi",
            "Test_AP"
        };
    }

    WIFIError ConnectViaWPS() override {
        std::cout << "[WIFI] Starting WPS pairing...\n";

        // Simulate WPS negotiation time
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "[WIFI] WPS connected successfully.\n";
        return WIFIAPI_SUCCESS;
    }

    WIFIError ConnectNormally(const std::string& ssid,
                              const std::string& password) override {
        std::cout << "[WIFI] Connecting to SSID: " << ssid << "\n";

        // Simulate connection attempt
        std::this_thread::sleep_for(std::chrono::seconds(3));

        if (ssid.empty()) {
            std::cout << "[WIFI] Network not reachable.\n";
            return WIFIAPI_ERROR_NOT_REACHABLE;
        }

        // Simulate authentication delay
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (password != "a") {
            std::cout << "[WIFI] Incorrect password.\n";
            return WIFIAPI_ERROR_INCORRECT_PASSWORD;
        }

        std::cout << "[WIFI] Connected successfully to " << ssid << ".\n";
        return WIFIAPI_SUCCESS;
    }
};

static Windows_WIFIAPI api;
WIFIAPI* _WIFIAPI = &api;

#endif