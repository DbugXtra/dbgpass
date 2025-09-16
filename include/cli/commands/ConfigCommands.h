#pragma once

#include "cli/commands/Command.h"

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Command to disable lowercase characters in password generation.
 */
class NoLowercaseCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to disable uppercase characters in password generation.
 */
class NoUppercaseCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to disable digits in password generation.
 */
class NoDigitsCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to disable symbols in password generation.
 */
class NoSymbolsCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to enable pronounceable password generation.
 */
class PronounceableCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to enable quiet mode.
 */
class QuietCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator