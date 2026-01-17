#pragma once

#include <wx/button.h>
#include <wx/panel.h>

class InstallPanel : public wxPanel {
public:
    wxButton* restartBtn;

    InstallPanel(wxWindow* parent);
};