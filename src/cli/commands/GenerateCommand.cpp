#include "cli/commands/GenerateCommand.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace password_generator {
namespace cli {
namespace commands {

void GenerateCommand::execute() {
    generator.setConfig(config);
    std::string password = generator.generate();
    
    std::cout << "\n┌─ Generated Password ─────────────────┐\n";
    std::cout << "│ " << std::setw(36) << std::left << password << " │\n";
    std::cout << "├──────────────────────────────────────┤\n";
    std::cout << "│ Length: " << std::setw(28) << std::left 
              << (std::to_string(password.length()) + " characters") << " │\n";
    
    // Calculate entropy
    size_t charSpace = 0;
    if (config.includeLowercase) charSpace += 26;
    if (config.includeUppercase) charSpace += 26;
    if (config.includeDigits) charSpace += 10;
    if (config.includeSymbols) charSpace += config.customSymbols.length();
    
    if (charSpace > 0) {
        double entropy = password.length() * std::log2(charSpace);
        std::cout << "│ Entropy: " << std::setw(28) << std::left 
                  << (std::to_string(static_cast<int>(entropy)) + " bits") << " │\n";
    }
    
    std::cout << "└──────────────────────────────────────┘\n\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator