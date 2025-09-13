#ifndef SET_SYMBOLS_COMMAND_H
#define SET_SYMBOLS_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class SetSymbolsCommand : public Command {
private:
    core::config::PasswordGeneratorConfig& config;
    
public:
    SetSymbolsCommand(core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // SET_SYMBOLS_COMMAND_H