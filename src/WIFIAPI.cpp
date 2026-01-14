#include "WIFIAPI.h"

#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/utils.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <wx/event.h>

WIFIAPI::WIFIAPI(CommandAPI& cmdAPI, ConsolePanel* console) : cmdAPI(cmdAPI), console(console) {   
}

std::vector<std::string> WIFIAPI::GetAvailableNetworks() {
    std::vector<std::string> networks;

    cmdAPI.SetOutputCallback([&](const std::string& line){
        console->AddLine(line);
    });

    cmdAPI.RunCommand("wpa_cli -i wlan0 scan");
    wxSleep(3);

    std::string output;
    cmdAPI.RunCommand("wpa_cli -i wlan0 scan_results", &output);

    std::istringstream iss(output);
    std::string line;

    std::getline(iss, line);

    while (std::getline(iss, line)) {
        std::string bssid, freq, signal, flags, ssid;
        std::istringstream ls(line);

        ls >> bssid >> freq >> signal >> flags >> ssid;
        std::getline(ls, line);
        ssid.erase(ssid.begin(), std::find_if(ssid.begin(), ssid.end(), [](unsigned char c){
            return std::isspace(c);
        }));

        wxMessageBox(ssid, "info", wxOK | wxICON_INFORMATION);

        networks.push_back(ssid);
    }

    return networks;
}

WIFIError WIFIAPI::ConnectViaWPS(const std::string& ssid) {
    return WIFIAPI_SUCCESS;
}

WIFIError WIFIAPI::ConnectNormally(const std::string& ssid, const std::string& password) {
    return WIFIAPI_SUCCESS;
}

/*
class Linux_WIFIAPI : public WIFIAPI {
public:
    std::vector<std::string> GetAvailableNetworks() override {
        Exec("wpa_cli -i wlan0 scan");
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::string output = Exec("wpa_cli -i wlan0 scan_results");
        std::vector<std::string> networks;
        std::istringstream iss(output);
        std::string line;

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

        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::string status = Exec("wpa_cli -i wlan0 status");
        if (status.find("wpa_state=COMPLETED") != std::string::npos)
            return WIFIAPI_SUCCESS;

        return WIFIAPI_ERROR_NOT_REACHABLE;
    }

    WIFIError ConnectNormally(const std::string& ssid,
                              const std::string& password) override {
        if (ssid.empty()) return WIFIAPI_ERROR_NOT_REACHABLE;

        std::string netIdStr = Exec("wpa_cli -i wlan0 add_network");
        int netId = std::stoi(netIdStr);

        Exec("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
             " ssid '\"" + ssid + "\"'");
        Exec("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
             " psk '\"" + password + "\"'");
        Exec("wpa_cli -i wlan0 enable_network " + std::to_string(netId));
        Exec("wpa_cli -i wlan0 save_config");

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
};*/