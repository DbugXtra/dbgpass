#include "cli/PasswordGeneratorCLI.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        password_generator::cli::PasswordGeneratorCLI cli;
        
        // Process command-line arguments
        int result = cli.processArgs(argc, argv);
        if (result == -1) {
            // No arguments provided, run interactive mode
            cli.run();
        } else {
            // Arguments were processed, return the result
            return result;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}