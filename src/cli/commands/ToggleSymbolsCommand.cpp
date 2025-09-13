#include "cli/commands/ToggleSymbolsCommand.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void ToggleSymbolsCommand::execute() {
    config.includeSymbols = !config.includeSymbols;
    std::cout << "✓ Symbols " << (config.includeSymbols ? "enabled" : "disabled") << "\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator