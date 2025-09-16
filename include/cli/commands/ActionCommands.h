#pragma once

#include "cli/commands/Command.h"

namespace password_generator {
namespace cli {
namespace commands {

/**
 * Command to display current configuration.
 */
class ConfigShowCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to generate a single password.
 */
class GenerateCommand : public Command {
public:
    int execute(CommandContext& context) override;
};

/**
 * Command to generate multiple passwords in batch.
 */
class BatchCommand : public Command {
private:
    size_t batchCount;
public:
    explicit BatchCommand(size_t count = 1) : batchCount(count) {}
    int execute(CommandContext& context) override;

    // Static factory method to create and parse batch count
    static std::unique_ptr<BatchCommand> create(CommandContext& context);
};

/**
 * Command to validate a password.
 */
class ValidateCommand : public Command {
private:
    std::string password;
public:
    explicit ValidateCommand(const std::string& pwd) : password(pwd) {}
    int execute(CommandContext& context) override;

    // Static factory method to create and parse password argument
    static std::unique_ptr<ValidateCommand> create(CommandContext& context);
};

} // namespace commands
} // namespace cli
} // namespace password_generator