#include "CommandAPI.h"

#include <cstdio>
#include <fmt/format.h>

void CommandAPI::AddOutputCallback(const CommandOutputCallback& callback) {
    outputCallbacks.push_back(callback);
}

int CommandAPI::RunCommand(const std::string& cmd) {
    char buf[256] = { 0 };

    OutputLine(fmt::format("Running: {}", cmd));

    FILE* p = popen((cmd + " 2>&1").c_str(), "r");

    while (fgets(buf, sizeof(buf), p) != NULL) {
        std::string line = buf;
        if (!line.empty() && line.back() == '\n') line.erase(line.end() - 1);

        OutputLine(line);
    }

    int ret = pclose(p);
    OutputLine(fmt::format("{}: exited with code: {}", cmd, ret));
    return ret;
}

void CommandAPI::OutputLine(const std::string& line) {
    for (const auto& callback : outputCallbacks) {
        callback(line);
    }
}
