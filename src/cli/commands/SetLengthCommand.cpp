#include "cli/commands/SetLengthCommand.h"
#include <iostream>
#include <string>

namespace password_generator {
namespace cli {
namespace commands {

void SetLengthCommand::execute() {
    std::cout << "Enter password length (8-128): ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        size_t length = std::stoul(input);
        if (length >= 8 && length <= 128) {
            config.length = length;
            std::cout << "✓ Password length set to " << length << "\n";
        } else {
            std::cout << "✗ Length must be between 8 and 128\n";
        }
    } catch (const std::exception&) {
        std::cout << "✗ Invalid number\n";
    }
}

} // namespace commands
} // namespace cli
} // namespace password_generator