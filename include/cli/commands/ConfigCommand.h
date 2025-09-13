#ifndef CONFIG_COMMAND_H
#define CONFIG_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class ConfigCommand : public Command {
private:
    const core::config::PasswordGeneratorConfig& config;
    
public:
    ConfigCommand(const core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // CONFIG_COMMAND_H