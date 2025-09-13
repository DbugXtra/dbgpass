#include "cli/commands/ToggleDigitsCommand.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void ToggleDigitsCommand::execute() {
    config.includeDigits = !config.includeDigits;
    std::cout << "✓ Digits " << (config.includeDigits ? "enabled" : "disabled") << "\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator