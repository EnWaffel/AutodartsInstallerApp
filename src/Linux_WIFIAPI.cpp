#ifdef _WIN32
#include "WIFIAPI.h"

#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <cstdio>
#include <thread>
#include <chrono>

class Linux_WIFIAPI : public WIFIAPI {
public:
    std::vector<std::string> GetAvailableNetworks() override {
        Exec("wpa_cli -i wlan0 scan");
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::string output = Exec("wpa_cli -i wlan0 scan_results");
        std::vector<std::string> networks;
        std::istringstream iss(output);
        std::string line;

        // Skip header line
        std::getline(iss, line);

        while (std::getline(iss, line)) {
            std::istringstream ls(line);
            std::string bssid, freq, signal, flags, ssid;

            ls >> bssid >> freq >> signal >> flags;
            std::getline(ls, ssid);
            if (!ssid.empty() && ssid[0] == ' ') ssid.erase(0, 1);

            if (!ssid.empty()) {
                networks.push_back(ssid);
            }
        }

        return networks;
    }

    WIFIError ConnectViaWPS() override {
        std::string result = Exec("wpa_cli -i wlan0 wps_pbc");
        if (result.find("OK") == std::string::npos)
            return WIFIAPI_ERROR_OTHER;

        // Wait for connection to establish
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::string status = Exec("wpa_cli -i wlan0 status");
        if (status.find("wpa_state=COMPLETED") != std::string::npos)
            return WIFIAPI_SUCCESS;

        return WIFIAPI_ERROR_NOT_REACHABLE;
    }

    WIFIError ConnectNormally(const std::string& ssid,
                              const std::string& password) override {
        if (ssid.empty()) return WIFIAPI_ERROR_NOT_REACHABLE;


        // Add network
        std::string netIdStr = Exec("wpa_cli -i wlan0 add_network");
        int netId = std::stoi(netIdStr);

        // Configure SSID and password
        Exec("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
             " ssid '\"" + ssid + "\"'");
        Exec("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
             " psk '\"" + password + "\"'");
        Exec("wpa_cli -i wlan0 enable_network " + std::to_string(netId));
        Exec("wpa_cli -i wlan0 save_config");

        // Wait and poll status
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::string status = Exec("wpa_cli -i wlan0 status");

            if (status.find("wpa_state=COMPLETED") != std::string::npos)
                return WIFIAPI_SUCCESS;

            if (status.find("WRONG_KEY") != std::string::npos)
                return WIFIAPI_ERROR_INCORRECT_PASSWORD;
        }

        return WIFIAPI_ERROR_NOT_REACHABLE;
    }

private:
    // Execute a shell command and return its stdout
    std::string Exec(const std::string& cmd) {
        std::array<char, 256> buffer{};
        std::string result;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return "";

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            std::string line(buffer.data());
            if (!line.empty() && line.back() == '\n') line.pop_back();
            result += line + "\n";
        }

        pclose(pipe);
        return result;
    }
};

// Global instance
static Linux_WIFIAPI api;
WIFIAPI* _WIFIAPI = &api;

#endif