#pragma once

#include "cli/commands/Command.h"
#include <memory>
#include <string>

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Command to set custom symbol characters for password generation.
 */
class SetSymbolsCommand : public Command {
private:
    std::string symbols;
public:
    explicit SetSymbolsCommand(const std::string& syms) : symbols(syms) {}
    int execute(CommandContext& context) override;

    // Static factory method to create and parse symbols argument
    static std::unique_ptr<SetSymbolsCommand> create(CommandContext& context);
};

} // namespace commands
} // namespace cli
} // namespace password_generator