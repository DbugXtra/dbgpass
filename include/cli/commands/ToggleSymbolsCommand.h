#ifndef TOGGLE_SYMBOLS_COMMAND_H
#define TOGGLE_SYMBOLS_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class ToggleSymbolsCommand : public Command {
private:
    core::config::PasswordGeneratorConfig& config;
    
public:
    ToggleSymbolsCommand(core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // TOGGLE_SYMBOLS_COMMAND_H