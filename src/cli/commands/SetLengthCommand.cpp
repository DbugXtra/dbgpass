#include "cli/commands/SetLengthCommand.h"
#include "cli/commands/CommandContext.h"
#include <iostream>
#include <stdexcept>
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

std::unique_ptr<SetLengthCommand> SetLengthCommand::create(CommandContext& context) {
    if (!context.hasNextArg()) {
        std::cerr << "Error: --length requires a value\n";
        return nullptr;
    }

    try {
        const std::string& lengthStr = context.getNextArg();
        size_t length = std::stoul(lengthStr);
        if (length >= 8 && length <= 128) {
            return std::make_unique<SetLengthCommand>(length);
        } else {
            std::cerr << "Error: Length must be between 8 and 128\n";
            return nullptr;
        }
    } catch (const std::exception&) {
        std::cerr << "Error: Invalid length value\n";
        return nullptr;
    }
}

int SetLengthCommand::execute(CommandContext& context) {
    context.config.length = length;
    return 0;
}

} // namespace commands
} // namespace cli
} // namespace password_generator