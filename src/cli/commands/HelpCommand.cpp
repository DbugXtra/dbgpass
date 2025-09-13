#include "cli/commands/HelpCommand.h"
#include <iostream>
#include <iomanip>

namespace password_generator {
namespace cli {
namespace commands {

void HelpCommand::execute() {
    std::cout << "\n=== Available Commands ===\n";
    std::cout << std::left;
    std::cout << std::setw(22) << "  generate" << "Generate a single password\n";
    std::cout << std::setw(22) << "  batch" << "Generate multiple passwords\n";
    std::cout << std::setw(22) << "  config" << "Show current configuration\n";
    std::cout << std::setw(22) << "  set length" << "Set password length\n";
    std::cout << std::setw(22) << "  toggle lowercase" << "Toggle lowercase characters\n";
    std::cout << std::setw(22) << "  toggle uppercase" << "Toggle uppercase characters\n";
    std::cout << std::setw(22) << "  toggle digits" << "Toggle digit characters\n";
    std::cout << std::setw(22) << "  toggle symbols" << "Toggle symbol characters\n";
    std::cout << std::setw(22) << "  toggle pronounceable" << "Toggle pronounceable mode\n";
    std::cout << std::setw(22) << "  set symbols" << "Set custom symbol set\n";
    std::cout << std::setw(22) << "  validate" << "Validate a password\n";
    std::cout << std::setw(22) << "  clear" << "Clear screen\n";
    std::cout << std::setw(22) << "  help" << "Show this help message\n";
    std::cout << std::setw(22) << "  exit/quit" << "Exit the program\n\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator