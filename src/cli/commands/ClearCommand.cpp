#include "cli/commands/ClearCommand.h"
#include <cstdlib>

namespace password_generator {
namespace cli {
namespace commands {

void ClearCommand::execute() {
    #ifdef _WIN32
        int result = system("cls");
        (void)result;
    #else
        int result = system("clear");
        (void)result;
    #endif
}

} // namespace commands
} // namespace cli
} // namespace password_generator