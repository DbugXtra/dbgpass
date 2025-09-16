#include "cli/commands/CommandInvoker.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void CommandInvoker::addCommand(std::unique_ptr<Command> command) {
    if (command) {
        commands.push_back(std::move(command));
    }
}

int CommandInvoker::executeCommands(CommandContext& context) {
    for (auto& command : commands) {
        int result = command->execute(context);
        if (result != 0) {
            // Command failed, return error code
            return result;
        }

        // If command requested early exit (e.g., help or version), honor it
        if (context.shouldExit) {
            return context.exitCode;
        }
    }

    return 0; // All commands executed successfully
}

void CommandInvoker::clear() {
    commands.clear();
}

size_t CommandInvoker::size() const {
    return commands.size();
}

} // namespace commands
} // namespace cli
} // namespace password_generator