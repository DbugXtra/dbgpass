#include "cli/commands/SetSymbolsCommand.h"
#include <iostream>
#include <string>

namespace password_generator {
namespace cli {
namespace commands {

void SetSymbolsCommand::execute() {
    std::cout << "Enter custom symbol set: ";
    std::string symbols;
    std::getline(std::cin, symbols);
    
    if (!symbols.empty()) {
        config.customSymbols = symbols;
        std::cout << "✓ Custom symbols set to: " << symbols << "\n";
    }
}

} // namespace commands
} // namespace cli
} // namespace password_generator