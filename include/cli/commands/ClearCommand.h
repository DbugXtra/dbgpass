#ifndef CLEAR_COMMAND_H
#define CLEAR_COMMAND_H

#include "cli/Command.h"

namespace password_generator {
namespace cli {
namespace commands {

class ClearCommand : public Command {
public:
    void execute() override;
};

} // namespace commands
} // namespace cli
} // namespace password_generator

#endif // CLEAR_COMMAND_H