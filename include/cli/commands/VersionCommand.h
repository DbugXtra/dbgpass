#pragma once

#include "cli/commands/Command.h"

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Command to display version information and exit.
 */
class VersionCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator