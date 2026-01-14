#pragma once

#include <wx/panel.h>
#include <wx/button.h>

class WPSConnectPanel : public wxPanel {
public:
    wxButton* okBtn;
    wxButton* nextBtn;
    wxButton* restartBtn;

    WPSConnectPanel(wxWindow* parent);
};