#pragma once

#include "cli/commands/Command.h"
#include "cli/commands/CommandContext.h"
#include <memory>
#include <vector>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Factory class responsible for creating command objects based on command line arguments.
 */
class CommandFactory {
public:
    /**
     * Parse command line arguments and create appropriate command objects.
     * @param context The command context containing arguments and state
     * @return Vector of command objects to execute, or empty vector if error
     */
    static std::vector<std::unique_ptr<Command>> createCommands(CommandContext& context);

private:
    /**
     * Create a command for the given argument.
     * @param arg The command line argument
     * @param context The command context
     * @return Command object or nullptr if argument is unknown
     */
    static std::unique_ptr<Command> createCommand(const std::string& arg, CommandContext& context);

    /**
     * Determine the default action command if no explicit action was specified.
     * @param commands The list of commands parsed so far
     * @return Default action command or nullptr
     */
    static std::unique_ptr<Command> createDefaultAction(const std::vector<std::unique_ptr<Command>>& commands);

    /**
     * Check if the commands list contains an action command.
     * @param commands The list of commands to check
     * @return True if an action command is present
     */
    static bool hasActionCommand(const std::vector<std::unique_ptr<Command>>& commands);
};

} // namespace commands
} // namespace cli
} // namespace password_generator