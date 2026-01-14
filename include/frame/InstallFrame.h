#pragma once

#include "panel/AskPwdPanel.h"
#include "panel/ConnectPanel.h"
#include "panel/ConsolePanel.h"
#include "panel/InstallPanel.h"
#include "panel/WIFISelectPanel.h"
#include "panel/WPSAskPanel.h"
#include "panel/WPSConnectPanel.h"
#include "CommandAPI.h"

#include <wx/event.h>
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <functional>
#include <wx/timer.h>

class InstallFrame : public wxFrame {
public:
    InstallFrame();
private:
    CommandAPI cmdAPI;

    wxString selectedNetwork;
    wxString password;
    wxTimer finishedTimer;

    wxPanel* panel = nullptr;
    wxBoxSizer* allSizer = nullptr;
    wxStaticText* titleText = nullptr;
    wxStaticText* subtitleText = nullptr;

    WIFISelectPanel* wifiSelectPanel = nullptr;
    WPSAskPanel* wpsAskPanel = nullptr;
    WPSConnectPanel* wpsConnectPanel = nullptr;
    AskPwdPanel* askPwdPanel = nullptr;
    ConnectPanel* connectPanel = nullptr;
    InstallPanel* installPanel = nullptr;
    ConsolePanel* consolePanel = nullptr;
    
    void OnWPSOkButton(wxCommandEvent& event);
    void OnWPSNextButton(wxCommandEvent& event);

    void CreateWIFISelectPanel();
    void CreateWPSAskPanel();
    void CreateWPSConnectPanel();
    void CreateAskPwdPanel();
    void CreateConnectPanel();
    void CreateInstallPanel();

    void DoNormalConnect();
    void DoInstall();

    void RunAsync(const std::function<void()> func);
};