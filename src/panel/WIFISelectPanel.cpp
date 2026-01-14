#include "panel/WIFISelectPanel.h"
#include "wx/sizer.h"

#include "WIFIAPI.h"

WIFISelectPanel::WIFISelectPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    refreshBtn = new wxButton(this, wxID_ANY, "Aktualisieren", wxDefaultPosition);
    sizer->Add(refreshBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

    wxArrayString items;
    for (const auto& v : _WIFIAPI->GetAvailableNetworks()) {
        items.Add(v);
    }

    wifiList = new wxListBox(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        items,
        wxALIGN_CENTER_HORIZONTAL
    );
    sizer->Add(wifiList, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(20));
    
    selectBtn = new wxButton(this, wxID_ANY, wxString::FromUTF8("Auswählen"), wxDefaultPosition, wxSize(100, 35));
    selectBtn->Enable(false);
    sizer->Add(selectBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(20));

    wifiList->Bind(wxEVT_LISTBOX, [this](wxCommandEvent&) {
        bool somethingSelected = wifiList->GetSelection() != wxNOT_FOUND;
        selectBtn->Enable(somethingSelected);
    });

    SetSizer(sizer);
}
