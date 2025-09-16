#pragma once

#include "cli/commands/Command.h"
#include "cli/commands/CommandContext.h"
#include <vector>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Invoker class that manages and executes commands.
 * This class follows the Command pattern invoker role.
 */
class CommandInvoker {
public:
    /**
     * Add a command to the execution queue.
     * @param command Command to add
     */
    void addCommand(std::unique_ptr<Command> command);

    /**
     * Execute all commands in the queue with the given context.
     * @param context The execution context
     * @return Exit code (0 for success, non-zero for error)
     */
    int executeCommands(CommandContext& context);

    /**
     * Clear all commands from the queue.
     */
    void clear();

    /**
     * Get the number of commands in the queue.
     * @return Number of commands
     */
    size_t size() const;

private:
    std::vector<std::unique_ptr<Command>> commands;
};

} // namespace commands
} // namespace cli
} // namespace password_generator