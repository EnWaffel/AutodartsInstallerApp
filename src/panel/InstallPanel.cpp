#include "panel/InstallPanel.h"
#include "wx/event.h"

#include <wx/sizer.h>

InstallPanel::InstallPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    SetSizer(sizer);
}
