#include "cli/PasswordGeneratorCLI.h"
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <vector>

namespace password_generator {
namespace cli {

class PasswordGeneratorCLI::Impl {
public:
    core::PasswordGenerator generator;
    core::config::PasswordGeneratorConfig config;
    std::map<std::string, std::function<void()>> commands;
    bool quietMode = false;
    
    Impl() {
        initializeCommands();
    }
    
    void initializeCommands() {
        commands["help"] = [this]() { showHelp(); };
        commands["generate"] = [this]() { generatePassword(); };
        commands["batch"] = [this]() { generateBatch(); };
        commands["config"] = [this]() { showConfig(); };
        commands["set length"] = [this]() { setLength(); };
        commands["toggle lowercase"] = [this]() { 
            toggleOption(config.includeLowercase, "Lowercase"); 
        };
        commands["toggle uppercase"] = [this]() { 
            toggleOption(config.includeUppercase, "Uppercase"); 
        };
        commands["toggle digits"] = [this]() { 
            toggleOption(config.includeDigits, "Digits"); 
        };
        commands["toggle symbols"] = [this]() { 
            toggleOption(config.includeSymbols, "Symbols"); 
        };
        commands["toggle pronounceable"] = [this]() { 
            toggleOption(config.pronounceable, "Pronounceable mode"); 
        };
        commands["set symbols"] = [this]() { setCustomSymbols(); };
        commands["validate"] = [this]() { validateUserPassword(); };
        commands["clear"] = [this]() { clearScreen(); };
    }
    
    void showHelp() {
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
    
    void generatePassword() {
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
    
    void generateBatch() {
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
    
    void showConfig() {
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
    
    void setLength() {
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
    
    void toggleOption(bool& option, const std::string& name) {
        option = !option;
        std::cout << "✓ " << name << " " << (option ? "enabled" : "disabled") << "\n";
    }
    
    void setCustomSymbols() {
        std::cout << "Enter custom symbol set: ";
        std::string symbols;
        std::getline(std::cin, symbols);
        
        if (!symbols.empty()) {
            config.customSymbols = symbols;
            std::cout << "✓ Custom symbols set to: " << symbols << "\n";
        }
    }
    
    void validateUserPassword() {
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
    
    void clearScreen() {
        #ifdef _WIN32
            int result = system("cls");
            (void)result; // Suppress unused variable warning
        #else
            int result = system("clear");
            (void)result; // Suppress unused variable warning
        #endif
    }
    
    void showUsage(const std::string& programName) {
        std::cout << "dbgpass v1.0.0 - Debug Industries Pass\n";
        std::cout << "Usage: " << programName << " [options]\n\n";
        std::cout << "Options:\n";
        std::cout << "  -h, --help              Show this help message\n";
        std::cout << "      --version           Show version information\n";
        std::cout << "  -g, --generate          Generate a single password\n";
        std::cout << "  -b, --batch <count>     Generate multiple passwords\n";
        std::cout << "  -l, --length <n>        Set password length (8-128)\n";
        std::cout << "      --no-lowercase      Exclude lowercase characters\n";
        std::cout << "      --no-uppercase      Exclude uppercase characters\n";
        std::cout << "      --no-digits         Exclude digit characters\n";
        std::cout << "      --no-symbols        Exclude symbol characters\n";
        std::cout << "  -s, --symbols <chars>   Set custom symbol set\n";
        std::cout << "  -p, --pronounceable     Generate pronounceable password\n";
        std::cout << "  -c, --config            Show current configuration\n";
        std::cout << "  -v, --validate <pass>   Validate a password\n";
        std::cout << "  -q, --quiet             Suppress prompts and decorations\n\n";
        std::cout << "Examples:\n";
        std::cout << "  " << programName << "                    # Interactive mode\n";
        std::cout << "  " << programName << " -g                 # Generate one password\n";
        std::cout << "  " << programName << " -g -l 20           # Generate 20-char password\n";
        std::cout << "  " << programName << " -b 5               # Generate 5 passwords\n";
        std::cout << "  " << programName << " -g --no-symbols    # No symbols\n";
        std::cout << "  " << programName << " -p -l 12           # Pronounceable 12-char password\n";
    }
    
    int processArgs(int argc, char* argv[]) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }
        
        if (args.empty()) {
            return -1; // No args, run interactive mode
        }
        
        bool generateFlag = false;
        bool batchFlag = false;
        bool configFlag = false;
        bool validateFlag = false;
        size_t batchCount = 1;
        std::string validatePassword;
        
        for (size_t i = 0; i < args.size(); ++i) {
            const std::string& arg = args[i];
            
            if (arg == "-h" || arg == "--help") {
                showUsage(argv[0]);
                return 0;
            }
            else if (arg == "--version") {
                std::cout << "dbgpass v1.0.0 - Debug Industries Pass\n";
                return 0;
            }
            else if (arg == "-g" || arg == "--generate") {
                generateFlag = true;
            }
            else if (arg == "-b" || arg == "--batch") {
                batchFlag = true;
                if (i + 1 < args.size()) {
                    try {
                        batchCount = std::stoul(args[++i]);
                        if (batchCount == 0 || batchCount > 100) {
                            std::cerr << "Error: Batch count must be between 1 and 100\n";
                            return 1;
                        }
                    } catch (const std::exception&) {
                        std::cerr << "Error: Invalid batch count\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Error: --batch requires a count argument\n";
                    return 1;
                }
            }
            else if (arg == "-l" || arg == "--length") {
                if (i + 1 < args.size()) {
                    try {
                        size_t length = std::stoul(args[++i]);
                        if (length >= 8 && length <= 128) {
                            config.length = length;
                        } else {
                            std::cerr << "Error: Length must be between 8 and 128\n";
                            return 1;
                        }
                    } catch (const std::exception&) {
                        std::cerr << "Error: Invalid length value\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Error: --length requires a value\n";
                    return 1;
                }
            }
            else if (arg == "--no-lowercase") {
                config.includeLowercase = false;
            }
            else if (arg == "--no-uppercase") {
                config.includeUppercase = false;
            }
            else if (arg == "--no-digits") {
                config.includeDigits = false;
            }
            else if (arg == "--no-symbols") {
                config.includeSymbols = false;
            }
            else if (arg == "-s" || arg == "--symbols") {
                if (i + 1 < args.size()) {
                    config.customSymbols = args[++i];
                    config.includeSymbols = true;
                } else {
                    std::cerr << "Error: --symbols requires a symbol set\n";
                    return 1;
                }
            }
            else if (arg == "-p" || arg == "--pronounceable") {
                config.pronounceable = true;
            }
            else if (arg == "-c" || arg == "--config") {
                configFlag = true;
            }
            else if (arg == "-v" || arg == "--validate") {
                validateFlag = true;
                if (i + 1 < args.size()) {
                    validatePassword = args[++i];
                } else {
                    std::cerr << "Error: --validate requires a password argument\n";
                    return 1;
                }
            }
            else if (arg == "-q" || arg == "--quiet") {
                quietMode = true;
            }
            else {
                std::cerr << "Error: Unknown option '" << arg << "'\n";
                std::cerr << "Use --help for usage information\n";
                return 1;
            }
        }
        
        // Validate configuration
        if (!config.includeLowercase && !config.includeUppercase && 
            !config.includeDigits && !config.includeSymbols) {
            std::cerr << "Error: At least one character type must be enabled\n";
            return 1;
        }
        
        // Execute requested action
        generator.setConfig(config);
        
        if (configFlag) {
            showConfig();
        }
        else if (validateFlag) {
            auto errors = generator.getValidationErrors(validatePassword);
            if (errors.empty()) {
                if (!quietMode) {
                    std::cout << "✓ Password is valid!\n";
                } else {
                    std::cout << "valid\n";
                }
            } else {
                if (!quietMode) {
                    std::cout << "✗ Password validation failed:\n";
                    for (const auto& error : errors) {
                        std::cout << "  - " << error << "\n";
                    }
                } else {
                    for (const auto& error : errors) {
                        std::cout << error << "\n";
                    }
                }
                return 1;
            }
        }
        else if (batchFlag) {
            auto passwords = generator.generateBatch(batchCount);
            if (!quietMode) {
                std::cout << "\n┌─ Generated " << batchCount << " Passwords ────────────\n";
                for (size_t i = 0; i < passwords.size(); ++i) {
                    std::cout << "│ " << std::setw(3) << std::right << (i + 1) << ". " 
                              << std::setw(30) << std::left << passwords[i] << "\n";
                }
            } else {
                for (const auto& password : passwords) {
                    std::cout << password << "\n";
                }
            }
        }
        else if (generateFlag || (!configFlag && !validateFlag)) {
            // Generate single password (default action)
            std::string password = generator.generate();
            if (!quietMode) {
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
                
                std::cout << "└──────────────────────────────────────┘\n";
            } else {
                std::cout << password << "\n";
            }
        }
        
        return 0;
    }
};

PasswordGeneratorCLI::PasswordGeneratorCLI()
    : pImpl(std::make_unique<Impl>()) {}

PasswordGeneratorCLI::~PasswordGeneratorCLI() = default;

int PasswordGeneratorCLI::processArgs(int argc, char* argv[]) {
    return pImpl->processArgs(argc, argv);
}

void PasswordGeneratorCLI::run() {
    if (!pImpl->quietMode) {
        std::cout << "╔══════════════════════════════════════╗\n";
        std::cout << "║      dbgpass v1.0.0                  ║\n";
        std::cout << "║  Debug Industries Pass               ║\n";
        std::cout << "║  Secure passwords, terminal‑first.   ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
        std::cout << "Type 'help' for available commands\n\n";
    }
    
    std::string command;
    while (true) {
        if (!pImpl->quietMode) {
            std::cout << "❯ ";
        }
        
        std::getline(std::cin, command);
        
        if (command == "exit" || command == "quit") {
            if (!pImpl->quietMode) {
                std::cout << "Goodbye! Stay secure! 🔐\n";
            }
            break;
        }
        
        if (!processCommand(command)) {
            if (!command.empty() && !pImpl->quietMode) {
                std::cout << "Unknown command. Type 'help' for available commands.\n";
            }
        }
    }
}

bool PasswordGeneratorCLI::processCommand(const std::string& command) {
    auto it = pImpl->commands.find(command);
    if (it != pImpl->commands.end()) {
        try {
            it->second();
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            return true;
        }
    }
    return false;
}

void PasswordGeneratorCLI::setQuietMode(bool quiet) {
    pImpl->quietMode = quiet;
}

} // namespace cli
} // namespace password_generator