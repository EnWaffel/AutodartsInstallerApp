#include "panel/ConsolePanel.h"
#include "wx/string.h"

#include <wx/colour.h>

ConsolePanel::ConsolePanel(wxWindow* parent) : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 400), wxHSCROLL | wxVSCROLL) {
    lines.reserve(64);
    SetScrollRate(5, 5);
    EnableScrolling(true, true);
    SetBackgroundColour(wxColor(50, 50, 50));
    sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
}

void ConsolePanel::AddLine(const std::string& line) {
    if (!line.empty() && line.front() == '\r') {
        lines.back()->SetLabelText(wxString::FromUTF8(line));
        return;
    }

    wxStaticText* text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8(line));
    text->SetForegroundColour(*wxWHITE);
    sizer->Add(text, 0, wxALL, 0);
    sizer->Layout();
    FitInside();
    Scroll(0, GetScrollRange(wxVERTICAL));

    lines.push_back(text);
}
