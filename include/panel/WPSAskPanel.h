#pragma once

#include <wx/panel.h>
#include <wx/button.h>

class WPSAskPanel : public wxPanel {
public:
    wxButton* noBtn;
    wxButton* yesBtn;

    WPSAskPanel(wxWindow* parent);
};