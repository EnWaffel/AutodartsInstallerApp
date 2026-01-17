#include "panel/InstallPanel.h"
#include "wx/event.h"

#include <wx/sizer.h>

InstallPanel::InstallPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    restartBtn = new wxButton(this, wxID_ANY, "System Neustarten");
    restartBtn->Hide();
    sizer->Add(restartBtn, 0, wxALIGN_CENTER_HORIZONTAL, FromDIP(20));

    SetSizer(sizer);
}
