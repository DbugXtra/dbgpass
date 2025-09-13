#include "cli/commands/TogglePronounceableCommand.h"
#include <iostream>

namespace password_generator {
namespace cli {
namespace commands {

void TogglePronounceableCommand::execute() {
    config.pronounceable = !config.pronounceable;
    std::cout << "✓ Pronounceable mode " << (config.pronounceable ? "enabled" : "disabled") << "\n";
}

} // namespace commands
} // namespace cli
} // namespace password_generator