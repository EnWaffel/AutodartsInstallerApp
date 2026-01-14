#pragma once

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>

class AskPwdPanel : public wxPanel {
public:
    wxTextCtrl* pwdField;
    wxButton* confBtn;

    AskPwdPanel(wxWindow* parent);
};