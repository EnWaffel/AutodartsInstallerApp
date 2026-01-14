#include "panel/ConnectPanel.h"

#include <wx/sizer.h>

ConnectPanel::ConnectPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    nextBtn = new wxButton(this, wxID_ANY, "Weiter", wxDefaultPosition, wxSize(100, 35));
    restartBtn = new wxButton(this, wxID_ANY, "Neustarten", wxDefaultPosition, wxSize(100, 35));

    nextBtn->Hide(); 
    restartBtn->Hide();

    sizer->Add(nextBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    sizer->Add(restartBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));

    SetSizer(sizer);
}
