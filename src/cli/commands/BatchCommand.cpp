#include "cli/commands/BatchCommand.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace password_generator {
namespace cli {
namespace commands {

void BatchCommand::execute() {
    std::cout << "How many passwords? ";
    std::string input;
    std::getline(std::cin, input);
    
    try {
        size_t count = std::stoul(input);
        if (count == 0 || count > 100) {
            std::cout << "Please enter a number between 1 and 100\n";
            return;
        }
        
        generator.setConfig(config);
        auto passwords = generator.generateBatch(count);
        
        std::cout << "\n┌─ Generated " << count << " Passwords ────────────\n";
        for (size_t i = 0; i < passwords.size(); ++i) {
            std::cout << "│ " << std::setw(3) << std::right << (i + 1) << ". " 
                      << std::setw(30) << std::left << passwords[i] << "\n";
        }
    } catch (const std::exception&) {
        std::cout << "Invalid number\n";
    }
}

} // namespace commands
} // namespace cli
} // namespace password_generator