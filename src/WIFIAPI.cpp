#include "WIFIAPI.h"

#include <cstddef>
#include <cstring>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/utils.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <wx/event.h>

extern "C" {
    #include <glib.h>
    #include <NetworkManager.h>
}

WIFIAPI::WIFIAPI(CommandAPI& cmdAPI, ConsolePanel* console) : cmdAPI(cmdAPI), console(console) {   
}

/*
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
            return !std::isspace(c);
        }));

        if (std::find(networks.begin(), networks.end(), ssid) == networks.end()) {
            networks.push_back(ssid);
        }
    }
*/

WIFIError WIFIAPI::GetAvailableNetworks(std::vector<std::string>& networks) {
    cmdAPI.SetOutputCallback([&](const std::string& line){
        console->AddLine(line);
    });

    GError* error = NULL;
    NMClient* client;

    client = nm_client_new(NULL, &error);
    if (!client) {
        static char buf[128];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "\n[ERROR] %s\n", error->message);
        console->AddLine(buf);
        return WIFIAPI_ERROR_OTHER;
    }

    const GPtrArray* devices = nm_client_get_devices(client);
    NMDevice* device = NULL;

    for (int i = 0; i < devices->len; i++) {
        device = (NMDevice*)devices->pdata[i];
        if (NM_IS_DEVICE_WIFI(device)) break;
    }

    if (!device) {
        g_object_unref(client);
        console->AddLine("\n[ERROR] No wifi device found!\n");
        return WIFIAPI_ERROR_OTHER;
    }

    const GPtrArray* aps = nm_device_wifi_get_access_points((NMDeviceWifi*)device);
    for (int i = 0; i < aps->len; i++) {
        NMAccessPoint* ap = (NMAccessPoint*)aps->pdata[i];
        GBytes* ssid;
        gsize len;
        const guint8* data = (const guint8*)g_bytes_get_data(ssid, &len);

        wxMessageBox(std::to_string(len).c_str(), "");

        ssid = nm_access_point_get_ssid(ap);
        char* ssidStr = nm_utils_ssid_to_utf8(data, len);

        networks.push_back(ssidStr);
    }

    g_object_unref(client);

    return WIFIAPI_SUCCESS;
}

WIFIError WIFIAPI::ConnectViaWPS(const std::string& ssid) {
    cmdAPI.SetOutputCallback([&](const std::string& line){
        console->AddLine(line);
    });

    std::string result;
    cmdAPI.RunCommand("wpa_cli -i wlan0 wps_pbc", &result);
    if (result.find("OK") == std::string::npos) return WIFIAPI_ERROR_OTHER;

    wxSleep(10);

    std::string status;
    cmdAPI.RunCommand("wpa_cli -i wlan0 status", &status);
    if (status.find("wpa_state=COMPLETED") != std::string::npos) return WIFIAPI_SUCCESS;

    return WIFIAPI_ERROR_NOT_REACHABLE;
}

WIFIError WIFIAPI::ConnectNormally(const std::string& ssid, const std::string& password) {
    if (ssid.empty()) return WIFIAPI_ERROR_NOT_REACHABLE;

    cmdAPI.SetOutputCallback([&](const std::string& line){
        console->AddLine(line);
    });

    std::string netIdStr;
    cmdAPI.RunCommand("wpa_cli -i wlan0 add_network", &netIdStr);
    int netId = std::stoi(netIdStr);

    cmdAPI.RunCommand("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
            " ssid '\"" + ssid + "\"'");
    cmdAPI.RunCommand("wpa_cli -i wlan0 set_network " + std::to_string(netId) +
            " psk '\"" + password + "\"'");
    cmdAPI.RunCommand("wpa_cli -i wlan0 enable_network " + std::to_string(netId));
    cmdAPI.RunCommand("wpa_cli -i wlan0 save_config");

    for (int i = 0; i < 10; ++i) {
        wxSleep(3);
        std::string status;
        cmdAPI.RunCommand("wpa_cli -i wlan0 status", &status);

        if (status.find("wpa_state=COMPLETED") != std::string::npos)
            return WIFIAPI_SUCCESS;

        if (status.find("WRONG_KEY") != std::string::npos)
            return WIFIAPI_ERROR_INCORRECT_PASSWORD;
    }

    return WIFIAPI_ERROR_NOT_REACHABLE;
}
