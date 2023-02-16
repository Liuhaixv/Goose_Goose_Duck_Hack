#pragma once
#include <string>
enum DebugLogLevel {
    EVENTS_ONLY,
    EVENTS_AND_FUNCTIONS_ONLY,
    ALL
};

enum DebugType {
    EVENT,
    FUNCTION,
    MEMORY_RADE,
    MEMORY_WRITE
};

class DebugConsole
{
public:
    std::string logStr;

    DebugConsole() {
        this->logLevel = EVENTS_ONLY;
    }

    DebugConsole(DebugLogLevel logLevel) {
        this->logLevel = logLevel;
    }

    void log(DebugType debugType);

private:
    DebugLogLevel logLevel = EVENTS_ONLY;

    void log();
};

