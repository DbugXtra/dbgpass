#ifndef SET_LENGTH_COMMAND_H
#define SET_LENGTH_COMMAND_H

#include "cli/Command.h"
#include "core/config/PasswordGeneratorConfig.h"

namespace password_generator {
namespace cli {
namespace commands {

class SetLengthCommand : public Command {
private:
    core::config::PasswordGeneratorConfig& config;
    
public:
    SetLengthCommand(core::config::PasswordGeneratorConfig& cfg)
        : config(cfg) {}
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // SET_LENGTH_COMMAND_H