#pragma once

#include <wx/wx.h>
#include <memory>
#include "CommandAPI.h"
#include "panel/ConsolePanel.h"

class DashboardFrame : public wxFrame {
public:
    DashboardFrame();
    ~DashboardFrame() = default;

private:
    wxPanel* panel;
    wxStaticText* statusText;
    wxButton* startButton;
    wxButton* stopButton;
    ConsolePanel* console;

    std::unique_ptr<CommandAPI> commandAPI;

    // Event handlers
    void OnStartClicked(wxCommandEvent& event);
    void OnStopClicked(wxCommandEvent& event);
    void UpdateStatus();

    wxDECLARE_EVENT_TABLE();
};
