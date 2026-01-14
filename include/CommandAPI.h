#pragma once

#include <string>
#include <functional>
#include <vector>

using CommandOutputCallback = std::function<void(const std::string&)>;

class CommandAPI {
public:
    void AddOutputCallback(const CommandOutputCallback& callback);

    int RunCommand(const std::string& cmd);
private:
    std::vector<CommandOutputCallback> outputCallbacks;

    void OutputLine(const std::string& line);
};