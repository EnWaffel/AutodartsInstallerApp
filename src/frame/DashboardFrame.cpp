#include "frame/DashboardFrame.h"

wxBEGIN_EVENT_TABLE(DashboardFrame, wxFrame)
    // Event table will be filled by Connect in constructor
wxEND_EVENT_TABLE()

DashboardFrame::DashboardFrame() 
    : wxFrame(nullptr, wxID_ANY, "Autodarts Dashboard", wxDefaultPosition, wxSize(600, 400)) 
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

    // Layout
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(titleText, 0, wxEXPAND | wxTOP | wxBOTTOM, 20);
    sizer->Add(statusText, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    sizer->Add(startButton, 0, wxEXPAND | wxALL, 5);
    sizer->Add(stopButton, 0, wxEXPAND | wxALL, 5);

    panel->SetSizer(sizer);

    // CommandAPI
    commandAPI = std::make_unique<CommandAPI>();
    commandAPI->SetOutputCallback([this](const std::string& line){
        CallAfter([this, line](){
            statusText->SetLabel(line);
        });
    });

    // Bind events
    startButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStartClicked, this);
    stopButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStopClicked, this);

    Centre();
    Show();

    // Initial status check
    UpdateStatus();
}

void DashboardFrame::UpdateStatus() {
    // Check systemctl status of autodarts
    commandAPI->RunCommand("systemctl is-active autodarts.service", nullptr);

    // Alternatively, you can capture output and set statusText in OutputLine
}

void DashboardFrame::OnStartClicked(wxCommandEvent& event) {
    statusText->SetLabel("Starting service...");
    commandAPI->RunCommand("sudo systemctl start autodarts.service", nullptr);
    UpdateStatus();
}

void DashboardFrame::OnStopClicked(wxCommandEvent& event) {
    statusText->SetLabel("Stopping service...");
    commandAPI->RunCommand("sudo systemctl stop autodarts.service", nullptr);
    UpdateStatus();
}
