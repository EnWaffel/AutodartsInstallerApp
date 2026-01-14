#pragma once

#include <wx/stattext.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>

class ConsolePanel : public wxScrolledWindow {
public:
    ConsolePanel(wxWindow* parent);

    void AddLine(const std::string& line);
private:
    wxSizer* sizer;
    std::vector<wxStaticText*> lines;
};