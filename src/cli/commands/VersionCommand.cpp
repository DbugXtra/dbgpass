#include "cli/commands/VersionCommand.h"
#include "cli/commands/CommandContext.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

int VersionCommand::execute(CommandContext& context) {
    std::cout << "dbgpass v1.0.0 - Debug Industries Pass\n";
    context.shouldExit = true;
    context.exitCode = 0;
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator