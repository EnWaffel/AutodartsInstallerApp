#pragma once

#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/panel.h>

class WIFISelectPanel : public wxPanel {
public:
    wxListBox* wifiList;
    wxButton* refreshBtn;
    wxButton* selectBtn;

    WIFISelectPanel(wxWindow* parent);
};