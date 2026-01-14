#pragma once

#include <string>
#include <functional>

using CommandOutputCallback = std::function<void(const std::string&)>;

class CommandAPI {
public:
    void SetOutputCallback(const CommandOutputCallback& callback);

    int RunCommand(const std::string& cmd, std::string* output = nullptr);
private:
    CommandOutputCallback outputCallback;

    void OutputLine(const std::string& line);
};