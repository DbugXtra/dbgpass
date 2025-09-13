#ifndef COMMAND_H
#define COMMAND_H

namespace password_generator {
namespace cli {

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

} // namespace cli
} // namespace password_generator

#endif // COMMAND_H