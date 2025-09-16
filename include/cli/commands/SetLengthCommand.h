#pragma once

#include "cli/commands/Command.h"
#include <memory>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Command to set password length from command line argument.
 */
class SetLengthCommand : public Command {
private:
    size_t length;
public:
    explicit SetLengthCommand(size_t len) : length(len) {}
    int execute(CommandContext& context) override;

    // Static factory method to create and parse length argument
    static std::unique_ptr<SetLengthCommand> create(CommandContext& context);
};

} // namespace commands
} // namespace cli
} // namespace password_generator