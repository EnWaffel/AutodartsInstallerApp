#pragma once

#include <wx/panel.h>
#include <wx/button.h>

class ConnectPanel : public wxPanel {
public:
    wxButton* nextBtn;
    wxButton* restartBtn;

    ConnectPanel(wxWindow* parent);
};