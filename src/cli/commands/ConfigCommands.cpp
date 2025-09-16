#include "cli/commands/ConfigCommands.h"
#include "cli/commands/CommandContext.h"

namespace password_generator {
namespace cli {
namespace commands {

int NoLowercaseCommand::execute(CommandContext& context) {
    context.config.includeLowercase = false;
    return 0;
}

int NoUppercaseCommand::execute(CommandContext& context) {
    context.config.includeUppercase = false;
    return 0;
}

int NoDigitsCommand::execute(CommandContext& context) {
    context.config.includeDigits = false;
    return 0;
}

int NoSymbolsCommand::execute(CommandContext& context) {
    context.config.includeSymbols = false;
    return 0;
}

int PronounceableCommand::execute(CommandContext& context) {
    context.config.pronounceable = true;
    return 0;
}

int QuietCommand::execute(CommandContext& context) {
    context.quietMode = true;
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator