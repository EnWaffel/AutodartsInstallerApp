#include "frame/DashboardFrame.h"

wxBEGIN_EVENT_TABLE(DashboardFrame, wxFrame)
wxEND_EVENT_TABLE()

DashboardFrame::DashboardFrame()
    : wxFrame(nullptr, wxID_ANY, "Autodarts Dashboard", wxDefaultPosition, wxSize(600, 500))
{
    panel = new wxPanel(this);

    // Title
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "Autodarts Service", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetPointSize(14);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(font);

    // Status
    statusText = new wxStaticText(panel, wxID_ANY, "Checking...", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    wxFont statusFont = statusText->GetFont();
    statusFont.SetPointSize(12);
    statusText->SetFont(statusFont);

    // Buttons
    startButton = new wxButton(panel, wxID_ANY, "Start Service");
    stopButton  = new wxButton(panel, wxID_ANY, "Stop Service");

    // Console
    console = new ConsolePanel(panel);

    // Layout
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(titleText, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    sizer->Add(statusText, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
    sizer->Add(startButton, 0, wxEXPAND | wxALL, 5);
    sizer->Add(stopButton, 0, wxEXPAND | wxALL, 5);
    sizer->Add(console, 1, wxEXPAND | wxALL, 10); // growable, fills remaining space

    panel->SetSizer(sizer);

    // CommandAPI
    commandAPI = std::make_unique<CommandAPI>();
    commandAPI->SetOutputCallback([this](const std::string& line){
        wxTheApp->CallAfter([this, line](){
            console->AddLine(line);
        });
    });

    // Bind button events
    startButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStartClicked, this);
    stopButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStopClicked, this);

    Centre();
    Show();

    // Initial status check
    UpdateStatus();
}

// Update the service status
void DashboardFrame::UpdateStatus()
{
    commandAPI->RunCommand("systemctl is-active autodarts.service", nullptr);
}

// Start button handler
void DashboardFrame::OnStartClicked(wxCommandEvent& event)
{
    statusText->SetLabel("Starting service...");
    commandAPI->RunCommand("sudo systemctl start autodarts.service", nullptr);
    UpdateStatus();
}

// Stop button handler
void DashboardFrame::OnStopClicked(wxCommandEvent& event)
{
    statusText->SetLabel("Stopping service...");
    commandAPI->RunCommand("sudo systemctl stop autodarts.service", nullptr);
    UpdateStatus();
}
