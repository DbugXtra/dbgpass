#pragma once

#include <memory>
#include <string>
#include <vector>

namespace password_generator {
namespace cli {
namespace commands {

// Forward declarations
class CommandContext;

/**
 * Abstract base class for all command pattern implementations.
 * Each command encapsulates a specific CLI operation.
 */
class Command {
public:
    /**
     * Execute the command with the given context.
     * @param context The command execution context
     * @return Exit code (0 for success, non-zero for error)
     */
    virtual int execute(CommandContext& context) = 0;

    /**
     * Virtual destructor for proper cleanup
     */
    virtual ~Command() = default;
};

} // namespace commands
} // namespace cli
} // namespace password_generator