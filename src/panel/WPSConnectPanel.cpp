#include "panel/WPSConnectPanel.h"

#include <wx/sizer.h>

WPSConnectPanel::WPSConnectPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    okBtn = new wxButton(this, wxID_ANY, "OK", wxDefaultPosition, wxSize(100, 35));
    nextBtn = new wxButton(this, wxID_ANY, "Weiter", wxDefaultPosition, wxSize(100, 35));
    restartBtn = new wxButton(this, wxID_ANY, "Neustarten", wxDefaultPosition, wxSize(100, 35));

    nextBtn->Hide(); 
    restartBtn->Hide();

    sizer->Add(okBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    sizer->Add(nextBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    sizer->Add(restartBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));

    SetSizer(sizer);
}
