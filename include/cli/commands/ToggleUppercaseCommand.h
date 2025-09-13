#ifndef TOGGLE_UPPERCASE_COMMAND_H
#define TOGGLE_UPPERCASE_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class ToggleUppercaseCommand : public Command {
private:
    core::config::PasswordGeneratorConfig& config;
    
public:
    ToggleUppercaseCommand(core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // TOGGLE_UPPERCASE_COMMAND_H