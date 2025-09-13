#ifndef TOGGLE_LOWERCASE_COMMAND_H
#define TOGGLE_LOWERCASE_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class ToggleLowercaseCommand : public Command {
private:
    core::config::PasswordGeneratorConfig& config;
    
public:
    ToggleLowercaseCommand(core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // TOGGLE_LOWERCASE_COMMAND_H