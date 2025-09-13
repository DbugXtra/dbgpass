#include "cli/commands/ConfigCommand.h"
#include <iostream>
#include <iomanip>

namespace password_generator {
namespace cli {
namespace commands {

void ConfigCommand::execute() {
    std::cout << "\n┌─ Current Configuration ──────────────┐\n";
    std::cout << "│ " << std::setw(20) << std::left << "Length:" 
              << std::setw(16) << std::right << config.length << " │\n";
    std::cout << "│ " << std::setw(20) << std::left << "Lowercase:" 
              << std::setw(16) << std::right << (config.includeLowercase ? "Yes" : "No") << " │\n";
    std::cout << "│ " << std::setw(20) << std::left << "Uppercase:" 
              << std::setw(16) << std::right << (config.includeUppercase ? "Yes" : "No") << " │\n";
    std::cout << "│ " << std::setw(20) << std::left << "Digits:" 
              << std::setw(16) << std::right << (config.includeDigits ? "Yes" : "No") << " │\n";
    std::cout << "│ " << std::setw(20) << std::left << "Symbols:" 
              << std::setw(16) << std::right << (config.includeSymbols ? "Yes" : "No") << " │\n";
    std::cout << "│ " << std::setw(20) << std::left << "Pronounceable:" 
              << std::setw(16) << std::right << (config.pronounceable ? "Yes" : "No") << " │\n";
    
    if (config.includeSymbols) {
        std::cout << "├──────────────────────────────────────┤\n";
        std::string symbolDisplay = config.customSymbols;
        if (symbolDisplay.length() > 30) {
            symbolDisplay = symbolDisplay.substr(0, 27) + "...";
        }
        std::cout << "│ Symbols: " << std::setw(27) << std::left << symbolDisplay << " │\n";
    }
    
    std::cout << "└──────────────────────────────────────┘\n\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator