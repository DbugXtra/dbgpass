#include "cli/commands/SetSymbolsCommand.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

std::unique_ptr<SetSymbolsCommand> SetSymbolsCommand::create(CommandContext& context) {
    if (!context.hasNextArg()) {
        std::cerr << "Error: --symbols requires a symbol set\n";
        return nullptr;
    }

    const std::string& symbols = context.getNextArg();
    return std::make_unique<SetSymbolsCommand>(symbols);
}

int SetSymbolsCommand::execute(CommandContext& context) {
    context.config.customSymbols = symbols;
    context.config.includeSymbols = true;
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator