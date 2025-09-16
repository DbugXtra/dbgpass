#include "cli/commands/CommandContext.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace password_generator {
namespace cli {
namespace commands {

CommandContext::CommandContext(const std::string& programName)
    : programName(programName) {}

bool CommandContext::hasNextArg() const {
    return currentArgIndex + 1 < args.size();
}

const std::string& CommandContext::getNextArg() {
    if (!hasNextArg()) {
        throw std::runtime_error("No more arguments available");
    }
    return args[++currentArgIndex];
}

const std::string& CommandContext::getCurrentArg() const {
    if (currentArgIndex >= args.size()) {
        throw std::runtime_error("No current argument available");
    }
    return args[currentArgIndex];
}

void CommandContext::advance() {
    if (currentArgIndex < args.size()) {
        currentArgIndex++;
    }
}

void CommandContext::showUsage() const {
    showUsageImpl();
}

void CommandContext::showConfig() const {
    showConfigImpl();
}

void CommandContext::showUsageImpl() const {
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
    std::cout << "  " << programName << " -g                 # Generate one password\n";
    std::cout << "  " << programName << " -g -l 20           # Generate 20-char password\n";
    std::cout << "  " << programName << " -b 5               # Generate 5 passwords\n";
    std::cout << "  " << programName << " -g --no-symbols    # No symbols\n";
    std::cout << "  " << programName << " -p -l 12           # Pronounceable 12-char password\n";
}

void CommandContext::showConfigImpl() const {
    if (!quietMode) {
        std::cout << "\n┌─ Current Configuration ──────────────┐\n";
        std::cout << "│ Length: " << std::setw(28) << std::left << config.length << " │\n";
        std::cout << "│ Lowercase: " << std::setw(25) << std::left << (config.includeLowercase ? "enabled" : "disabled") << " │\n";
        std::cout << "│ Uppercase: " << std::setw(25) << std::left << (config.includeUppercase ? "enabled" : "disabled") << " │\n";
        std::cout << "│ Digits: " << std::setw(28) << std::left << (config.includeDigits ? "enabled" : "disabled") << " │\n";
        std::cout << "│ Symbols: " << std::setw(27) << std::left << (config.includeSymbols ? "enabled" : "disabled") << " │\n";
        std::cout << "│ Pronounceable: " << std::setw(21) << std::left << (config.pronounceable ? "enabled" : "disabled") << " │\n";
        if (config.includeSymbols) {
            std::cout << "│ Custom symbols: " << std::setw(20) << std::left << config.customSymbols << " │\n";
        }
        std::cout << "└──────────────────────────────────────┘\n";
    } else {
        std::cout << "length=" << config.length << "\n";
        std::cout << "lowercase=" << (config.includeLowercase ? "true" : "false") << "\n";
        std::cout << "uppercase=" << (config.includeUppercase ? "true" : "false") << "\n";
        std::cout << "digits=" << (config.includeDigits ? "true" : "false") << "\n";
        std::cout << "symbols=" << (config.includeSymbols ? "true" : "false") << "\n";
        std::cout << "pronounceable=" << (config.pronounceable ? "true" : "false") << "\n";
        if (config.includeSymbols) {
            std::cout << "custom_symbols=" << config.customSymbols << "\n";
        }
    }
}

} // namespace commands
} // namespace cli
} // namespace password_generator