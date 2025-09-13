#ifndef VALIDATE_COMMAND_H
#define VALIDATE_COMMAND_H

#include "cli/Command.h"
#include "core/PasswordGenerator.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class ValidateCommand : public Command {
private:
    core::PasswordGenerator& generator;
    core::config::PasswordGeneratorConfig& config;
    
public:
    ValidateCommand(core::PasswordGenerator& gen, core::config::PasswordGeneratorConfig& cfg)
        : generator(gen), config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // VALIDATE_COMMAND_H