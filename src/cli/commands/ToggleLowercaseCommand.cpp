#include "cli/commands/ToggleLowercaseCommand.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void ToggleLowercaseCommand::execute() {
    config.includeLowercase = !config.includeLowercase;
    std::cout << "✓ Lowercase " << (config.includeLowercase ? "enabled" : "disabled") << "\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator