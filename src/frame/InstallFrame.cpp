#include "frame/InstallFrame.h"
#include "WIFIAPI.h"
#include "panel/AskPwdPanel.h"
#include "panel/ConnectPanel.h"
#include "panel/ConsolePanel.h"
#include "panel/InstallPanel.h"
#include "panel/WIFISelectPanel.h"
#include "panel/WPSAskPanel.h"
#include "panel/WPSConnectPanel.h"
#include "wx/timer.h"

#include <cstdlib>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/textctrl.h>
#include <wx/textctrl.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <thread>

InstallFrame::InstallFrame() : wxFrame(nullptr, wxID_ANY, "Installer", wxDefaultPosition, wxSize(800, 600)) {
    panel = new wxPanel(this);

    titleText = new wxStaticText(
        panel,
        wxID_ANY,
        "Installation",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER_HORIZONTAL
    );

    subtitleText = new wxStaticText(
        panel,
        wxID_ANY,
        "Suche nach WLAN Netzwerken...",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER_HORIZONTAL
    );

    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.Scale(5);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(font);

    wxFont font1 = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font1.Scale(2);
    subtitleText->SetFont(font1);

    const int SUBTITLE_WIDTH = FromDIP(1000);
    subtitleText->SetMinSize(wxSize(SUBTITLE_WIDTH, -1));
    subtitleText->SetMaxSize(wxSize(SUBTITLE_WIDTH, -1));
    subtitleText->Wrap(SUBTITLE_WIDTH);

    consolePanel = new ConsolePanel(panel);

    allSizer = new wxBoxSizer(wxVERTICAL);
    allSizer->Add(titleText, 0, wxEXPAND | wxTOP | wxBOTTOM, FromDIP(20));
    allSizer->Add(subtitleText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    allSizer->Add(consolePanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->SetSizer(allSizer);
    
    consolePanel->Hide();

    Centre();
    //ShowFullScreen(true);
    Show();

    Layout();
    Update();
    Refresh();

    CreateWIFISelectPanel();
}

void InstallFrame::CreateWIFISelectPanel() {
    subtitleText->SetLabelText(wxString::FromUTF8("Bitte wähle dein WLAN-Netwerk aus:"));
    allSizer->Layout();
    Layout();

    wifiSelectPanel = new WIFISelectPanel(panel);
    wifiSelectPanel->refreshBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
        wifiSelectPanel->Destroy();
        subtitleText->SetLabelText("Suche nach WLAN Netzwerken...");
        allSizer->Layout();
        Layout();
        Update();
        Refresh();

        CallAfter([this]{CreateWIFISelectPanel();});
    });

    wifiSelectPanel->selectBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&){
        selectedNetwork = wifiSelectPanel->wifiList->GetStringSelection();
        wifiSelectPanel->Destroy();
        wifiSelectPanel = nullptr;
        CreateWPSAskPanel();
    });

    consolePanel->Show();
    RunAsync([&]{
        WIFIAPI wifiAPI(cmdAPI, consolePanel);
    
        wxArrayString items;
        for (const auto& v : wifiAPI.GetAvailableNetworks()) {
            items.Add(v);
        }
        
        CallAfter([this, items]{
            //consolePanel->Hide();
            //CreateWIFISelectPanel();
            //wifiSelectPanel->wifiList->Set(items);
        });
    });
    
    allSizer->Add(wifiSelectPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();
}

void InstallFrame::CreateWPSAskPanel() {
    wpsAskPanel = new WPSAskPanel(panel);
    
    subtitleText->SetLabelText(wxString::Format(wxString::FromUTF8("Soll über WPS mit dem Netzwerk verbunden werden?\n(%s)"), selectedNetwork));
    allSizer->Layout();
    Layout();

    wpsAskPanel->noBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&){
        wpsAskPanel->Destroy();
        wpsAskPanel = nullptr;
        CreateAskPwdPanel();
    });

    wpsAskPanel->yesBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&){
        wpsAskPanel->Destroy();
        wpsAskPanel = nullptr;
        CreateWPSConnectPanel();
    });

    allSizer->Add(wpsAskPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();
}

void InstallFrame::CreateWPSConnectPanel() {
    wpsConnectPanel = new WPSConnectPanel(panel);
    
    subtitleText->SetLabelText(wxString::FromUTF8("Jetzt WPS aktivieren und dann OK drücken!"));
    allSizer->Layout();
    Layout();

    wpsConnectPanel->okBtn->Bind(wxEVT_BUTTON, &InstallFrame::OnWPSOkButton, this);
    wpsConnectPanel->nextBtn->Bind(wxEVT_BUTTON, &InstallFrame::OnWPSNextButton, this);

    allSizer->Add(wpsConnectPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();
}

void InstallFrame::OnWPSOkButton(wxCommandEvent& event) {
    subtitleText->SetLabelText(wxString::Format(wxString::FromUTF8("Verbinde zu: %s..."), selectedNetwork));
    allSizer->Layout();
    Layout();
    wpsConnectPanel->okBtn->Hide();

    RunAsync([this]{
        WIFIAPI wifiAPI(cmdAPI, nullptr);
        WIFIError error = wifiAPI.ConnectViaWPS(selectedNetwork.utf8_string());

        if (error == WIFIAPI_SUCCESS) {
            CallAfter([this]() { subtitleText->SetLabel("Verbunden!"); allSizer->Layout(); Layout(); });
        } else if (error == WIFIAPI_ERROR_NOT_REACHABLE) {
            CallAfter([this]() { subtitleText->SetLabel("Verbindung fehlgeschlagen: Netzwerk nicht erreichbar!"); allSizer->Layout(); Layout(); });
        } else {
            CallAfter([this]() { subtitleText->SetLabel("Verbindung fehlgeschlagen: Unbekannter error"); allSizer->Layout(); Layout(); });
        }

        if (error == WIFIAPI_SUCCESS) {
            CallAfter([this]() {
                wpsConnectPanel->nextBtn->Show();
                allSizer->Layout();
                Layout();
                Update();
                Refresh();
            });
        } else {
            CallAfter([this]() {
                wpsConnectPanel->restartBtn->Show();
                wpsConnectPanel->Layout();
                wpsConnectPanel->restartBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
                    wpsConnectPanel->Destroy();
                    wpsConnectPanel = nullptr;
                    CreateWIFISelectPanel();
                });
                allSizer->Layout();
                Layout();
                Update();
                Refresh();
            });
        }
    });
}

void InstallFrame::OnWPSNextButton(wxCommandEvent& event) {
    wpsConnectPanel->Destroy();
    wpsConnectPanel = nullptr;
    CreateInstallPanel();
}

void InstallFrame::CreateAskPwdPanel() {
    askPwdPanel = new AskPwdPanel(panel);

    subtitleText->SetLabelText(wxString::Format(wxString::FromUTF8("Bitte das Passwort vom WLAN eingeben:\n(%s)"), selectedNetwork));
    allSizer->Layout();
    Layout();
    
    askPwdPanel->confBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent&){
        password = askPwdPanel->pwdField->GetValue();
        askPwdPanel->Destroy();
        CallAfter([&]{CreateConnectPanel();});
    });

    allSizer->Add(askPwdPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();
}

void InstallFrame::CreateConnectPanel() {
    connectPanel = new ConnectPanel(panel);

    subtitleText->SetLabelText(wxString::Format(wxString::FromUTF8("Verbinde zu: %s..."), selectedNetwork));
    allSizer->Layout();
    Layout();

    allSizer->Add(connectPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();

    DoNormalConnect();
}

void InstallFrame::CreateInstallPanel() {
    installPanel = new InstallPanel(panel);
    
    subtitleText->SetLabelText(wxString::Format(wxString::FromUTF8("Installiere..."), selectedNetwork));
    allSizer->Layout();
    Layout();
    
    allSizer->Add(installPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    panel->Layout();

    DoInstall();
}

void InstallFrame::DoNormalConnect() {
    RunAsync([&]{
        WIFIAPI wifiAPI(cmdAPI, nullptr);
        WIFIError error = wifiAPI.ConnectNormally(selectedNetwork.utf8_string(), password.utf8_string());

        if (error == WIFIAPI_SUCCESS) {
            CallAfter([this]() {
                connectPanel->nextBtn->Show();
                connectPanel->Layout();
                connectPanel->nextBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
                    connectPanel->Destroy();
                    connectPanel = nullptr;
                    CreateInstallPanel();
                });
            });
        } else {
            CallAfter([this]() {
                connectPanel->restartBtn->Show();
                connectPanel->Layout();
                connectPanel->restartBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&){
                    connectPanel->Destroy();
                    connectPanel = nullptr;
                    CreateWIFISelectPanel();
                });
            });
        }

        if (error == WIFIAPI_SUCCESS) {
            CallAfter([this]() { subtitleText->SetLabel("Verbunden!"); allSizer->Layout(); Layout(); });
        } else if (error == WIFIAPI_ERROR_NOT_REACHABLE) {
            CallAfter([this]() { subtitleText->SetLabel("Verbindung fehlgeschlagen: Netzwerk nicht erreichbar!"); allSizer->Layout(); Layout(); });
        } else if (error == WIFIAPI_ERROR_INCORRECT_PASSWORD) {
            CallAfter([this]() { subtitleText->SetLabel("Verbindung fehlgeschlagen: Falsches Passwort!"); allSizer->Layout(); Layout(); });
        } else {
            CallAfter([this]() { subtitleText->SetLabel("Verbindung fehlgeschlagen: Unbekannter error"); allSizer->Layout(); Layout(); });
        }
    });
}

void InstallFrame::DoInstall() {
    consolePanel->Show();

    RunAsync([this]{
        cmdAPI.AddOutputCallback([this](const std::string& line){
            CallAfter([this, line]{
                consolePanel->AddLine(line);
            });
        });

        cmdAPI.RunCommand("sudo apt-get update");
        
        if (cmdAPI.RunCommand("sudo dpkg -s curl") != EXIT_SUCCESS) {
            cmdAPI.RunCommand("sudo apt-get install curl -y");
        }

        cmdAPI.RunCommand("curl -L -o installAutodartsServer.sh get.autodarts.io");
        cmdAPI.RunCommand("chmod +x installAutodartsServer.sh");
        cmdAPI.RunCommand("sudo bash installAutodartsServer.sh");

        CallAfter([this]() {
            subtitleText->SetLabel("Alles Installiert!");
            allSizer->Layout();
            Layout();

            finishedTimer.StartOnce(3000);
            finishedTimer.Bind(wxEVT_TIMER, [&](wxTimerEvent&){
                installPanel->Destroy();
                installPanel = nullptr;
            });
        });
    });
}

void InstallFrame::RunAsync(const std::function<void()> func) {
    std::thread(func).detach();
}
