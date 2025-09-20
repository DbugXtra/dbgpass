#include "cli/commands/ActionCommands.h"
#include "cli/commands/CommandContext.h"

namespace password_generator {
namespace cli {
namespace commands {

int ConfigShowCommand::execute(CommandContext& context) {
    context.showConfig();
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator