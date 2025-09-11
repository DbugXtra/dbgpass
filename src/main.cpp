#include "cli/PasswordGeneratorCLI.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        password_generator::cli::PasswordGeneratorCLI cli;
        
        // Check for command-line arguments
        if (argc > 1) {
            std::string command;
            for (int i = 1; i < argc; ++i) {
                if (i > 1) command += " ";
                command += argv[i];
            }
            
            if (command == "--help" || command == "-h") {
                std::cout << "Secure Password Generator\n";
                std::cout << "Usage: passgen [options]\n";
                std::cout << "Options:\n";
                std::cout << "  --help, -h    Show this help message\n";
                std::cout << "  --version     Show version information\n";
                std::cout << "  (no args)     Start interactive mode\n";
                return 0;
            }
            
            if (command == "--version") {
                std::cout << "Secure Password Generator v1.0.0\n";
                return 0;
            }
        }
        
        // Run interactive mode
        cli.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}