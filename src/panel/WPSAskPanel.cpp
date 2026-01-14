#include "panel/WPSAskPanel.h"

#include <wx/sizer.h>

WPSAskPanel::WPSAskPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    noBtn = new wxButton(this, wxID_ANY, "Nein", wxDefaultPosition, wxSize(100, 35));
    yesBtn = new wxButton(this, wxID_ANY, "Ja", wxDefaultPosition, wxSize(100, 35));

    sizer->Add(noBtn, 0, wxALL, FromDIP(20));
    sizer->Add(yesBtn, 0, wxALL, FromDIP(20));

    this->SetSizer(sizer);
}
