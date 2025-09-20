#pragma once

#include "cli/commands/Command.h"
#include "cli/commands/CommandContext.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Builder pattern implementation for creating commands.
 * Uses a registry to eliminate if/else chains in command creation.
 */
class CommandBuilder {
public:
    using CommandCreator = std::function<std::unique_ptr<Command>(CommandContext&)>;

    /**
     * Register a command creator for the given argument(s).
     */
    CommandBuilder& registerCommand(const std::vector<std::string>& args, CommandCreator creator);

    /**
     * Create a command for the given argument.
     * @param arg The command line argument
     * @param context The command context
     * @return Command object or nullptr if argument is unknown
     */
    std::unique_ptr<Command> createCommand(const std::string& arg, CommandContext& context) const;

    /**
     * Get the singleton instance of the command builder.
     */
    static CommandBuilder& getInstance();

private:
    std::unordered_map<std::string, CommandCreator> commandRegistry;

    /**
     * Initialize the command registry with all supported commands.
     */
    void initializeRegistry();

    CommandBuilder();
};

} // namespace commands
} // namespace cli
} // namespace password_generator