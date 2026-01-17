#include "frame/DashboardFrame.h"
#include "wx/wx.h"

wxBEGIN_EVENT_TABLE(DashboardFrame, wxFrame)
wxEND_EVENT_TABLE()

DashboardFrame::DashboardFrame() : wxFrame(nullptr, wxID_ANY, "Autodarts Dashboard", wxDefaultPosition, wxSize(600, 500)) {
    panel = new wxPanel(this);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "Autodarts Service", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.Scale(4);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(font);

    statusText = new wxStaticText(panel, wxID_ANY, "Checking...", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    wxFont statusFont = statusText->GetFont();
    statusFont.SetPointSize(12);
    statusText->SetFont(statusFont);

    startButton = new wxButton(panel, wxID_ANY, "Start Service");
    stopButton  = new wxButton(panel, wxID_ANY, "Stop Service");

    console = new ConsolePanel(panel);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(titleText, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    sizer->Add(statusText, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
    sizer->Add(startButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    sizer->Add(stopButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    sizer->Add(console, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    panel->SetSizer(sizer);

    commandAPI = std::make_unique<CommandAPI>();
    commandAPI->SetOutputCallback([this](const std::string& line){
        wxTheApp->CallAfter([this, line](){
            console->AddLine(line);
        });
    });

    startButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStartClicked, this);
    stopButton->Bind(wxEVT_BUTTON, &DashboardFrame::OnStopClicked, this);

    Centre();
    Show();

    UpdateStatus();
}

void DashboardFrame::UpdateStatus()
{
    commandAPI->RunCommand("systemctl is-active autodarts.service");
}

void DashboardFrame::OnStartClicked(wxCommandEvent& event)
{
    statusText->SetLabel("STATUS: Starting...");
    Layout();
    Update();
    Refresh();
    commandAPI->RunCommand("sudo systemctl start autodarts.service");
    UpdateStatus();
    statusText->SetLabel("STATUS: Stopped");
    Layout();
    Update();
    Refresh();
}

void DashboardFrame::OnStopClicked(wxCommandEvent& event)
{
    statusText->SetLabel("STATUS: Stopping...");
    Layout();
    Update();
    Refresh();
    commandAPI->RunCommand("sudo systemctl stop autodarts.service");
    UpdateStatus();
    statusText->SetLabel("STATUS: Running");
    Layout();
    Update();
    Refresh();
}
