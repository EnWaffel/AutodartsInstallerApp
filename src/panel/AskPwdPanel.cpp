#include "panel/AskPwdPanel.h"

#include <wx/sizer.h>

AskPwdPanel::AskPwdPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    pwdField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, 25), wxTE_PASSWORD);
    confBtn = new wxButton(this, wxID_ANY, wxString::FromUTF8("Bestätigen"), wxDefaultPosition, wxSize(100, 35));

    sizer->Add(pwdField, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));
    sizer->Add(confBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, FromDIP(20));

    SetSizer(sizer);
}
