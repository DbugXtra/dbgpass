#include "cli/commands/HelpCommand.h"
#include "cli/commands/CommandContext.h"

namespace password_generator {
namespace cli {
namespace commands {

int HelpCommand::execute(CommandContext& context) {
    context.showUsage();
    context.shouldExit = true;
    context.exitCode = 0;
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator