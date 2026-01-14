#include "panel/InstallPanel.h"
#include "panel/ConsolePanel.h"
#include "wx/event.h"

#include <wx/sizer.h>

InstallPanel::InstallPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    this->console = new ConsolePanel(this);
    sizer->Add(console, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);
}
