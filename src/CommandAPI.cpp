#include "CommandAPI.h"

#include <cstdio>
#include <fmt/format.h>

void CommandAPI::SetOutputCallback(const CommandOutputCallback& callback) {
    outputCallback = callback;
}

int CommandAPI::RunCommand(const std::string& cmd, std::string* output) {
    char buf[256] = { 0 };
    std::string out;
    out.reserve(1024);

    OutputLine(fmt::format("Running: {}", cmd));

    FILE* p = popen((cmd + " 2>&1").c_str(), "r");

    while (fgets(buf, sizeof(buf), p) != NULL) {
        std::string line = buf;
        if (!line.empty() && line.back() == '\n') line.erase(line.end() - 1);

        out.append(line);
        OutputLine(line);
    }
    
    if (output) *output = out;

    int ret = pclose(p);
    OutputLine(fmt::format("{}: exited with code: {}", cmd, ret));
    return ret;
}

void CommandAPI::OutputLine(const std::string& line) {
    if (outputCallback) outputCallback(line);
}
