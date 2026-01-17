#pragma once

#include <wx/wx.h>
#include <memory>
#include "CommandAPI.h"

class DashboardFrame : public wxFrame {
public:
    DashboardFrame();
    ~DashboardFrame() = default;

private:
    wxPanel* panel;
    wxStaticText* statusText;
    wxButton* startButton;
    wxButton* stopButton;

    std::unique_ptr<CommandAPI> commandAPI;

    // Event handlers
    void OnStartClicked(wxCommandEvent& event);
    void OnStopClicked(wxCommandEvent& event);
    void UpdateStatus();

    wxDECLARE_EVENT_TABLE();
};
