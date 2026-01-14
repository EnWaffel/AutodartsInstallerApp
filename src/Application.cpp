#include "Application.h"
#include "frame/InstallFrame.h"
#include "wx/app.h"

#include <wx/init.h>
#include <filesystem>

namespace fs = std::filesystem;

bool Application::OnInit() {
    if (fs::exists(".installed")) {
    } else {
        InstallFrame* frame = new InstallFrame();
        frame->Show();
    }

    return true;
}

wxIMPLEMENT_APP_CONSOLE(Application);
