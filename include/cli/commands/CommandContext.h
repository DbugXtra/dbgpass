#pragma once

#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <string>
#include <vector>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Context object that holds shared state for command execution.
 * This allows commands to access and modify the application state.
 */
class CommandContext {
public:
    // Constructor
    CommandContext(const std::string& programName);

    // Core components
    core::PasswordGenerator generator;
    core::config::PasswordGeneratorConfig config;

    // CLI state
    bool quietMode = false;
    std::string programName;

    // Argument processing state
    std::vector<std::string> args;
    size_t currentArgIndex = 0;

    // Command execution state
    bool shouldExit = false;
    int exitCode = 0;

    // Helper methods for argument processing
    bool hasNextArg() const;
    const std::string& getNextArg();
    const std::string& getCurrentArg() const;
    void advance();

    // Helper methods for output
    void showUsage() const;
    void showConfig() const;

private:
    void showUsageImpl() const;
    void showConfigImpl() const;
};

} // namespace commands
} // namespace cli
} // namespace password_generator