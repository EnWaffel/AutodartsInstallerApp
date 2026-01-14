#pragma once

#include "panel/ConsolePanel.h"

#include <wx/panel.h>

class InstallPanel : public wxPanel {
public:
    ConsolePanel* console = nullptr;

    InstallPanel(wxWindow* parent);
};