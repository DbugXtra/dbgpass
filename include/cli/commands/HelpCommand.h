#ifndef HELP_COMMAND_H
#define HELP_COMMAND_H

#include "cli/Command.h"

namespace password_generator {
namespace cli {
namespace commands {

class HelpCommand : public Command {
public:
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // HELP_COMMAND_H