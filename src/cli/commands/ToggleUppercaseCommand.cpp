#include "cli/commands/ToggleUppercaseCommand.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void ToggleUppercaseCommand::execute() {
    config.includeUppercase = !config.includeUppercase;
    std::cout << "✓ Uppercase " << (config.includeUppercase ? "enabled" : "disabled") << "\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator