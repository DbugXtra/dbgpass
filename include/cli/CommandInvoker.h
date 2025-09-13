#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include <map>
#include <string>
#include <memory>

namespace password_generator {
namespace cli {

class Command;

class CommandInvoker {
public:
    void registerCommand(const std::string& commandName, std::shared_ptr<Command> command);
    bool executeCommand(const std::string& commandName);

private:
    std::map<std::string, std::shared_ptr<Command>> commands;
};

} // namespace cli
} // namespace password_generator

#endif // COMMANDINVOKER_H