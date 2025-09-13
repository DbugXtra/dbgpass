#include "cli/PasswordGeneratorCLI.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        password_generator::cli::PasswordGeneratorCLI cli;
        
        // Process command-line arguments
        int result = cli.processArgs(argc, argv);
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}