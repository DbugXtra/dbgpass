#include "cli/commands/ValidateCommand.h"
#include <iostream>
#include <string>

namespace password_generator {
namespace cli {
namespace commands {

void ValidateCommand::execute() {
    std::cout << "Enter password to validate: ";
    std::string password;
    std::getline(std::cin, password);
    
    generator.setConfig(config);
    auto errors = generator.getValidationErrors(password);
    
    if (errors.empty()) {
        std::cout << "✓ Password is valid!\n";
    } else {
        std::cout << "✗ Password validation failed:\n";
        for (const auto& error : errors) {
            std::cout << "  - " << error << "\n";
        }
    }
}

} // namespace commands
} // namespace cli
} // namespace password_generator