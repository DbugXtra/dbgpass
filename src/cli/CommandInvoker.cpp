#include "cli/CommandInvoker.h"
#include "cli/Command.h"
#include <iostream>

namespace password_generator {
namespace cli {

void CommandInvoker::registerCommand(const std::string& commandName, std::shared_ptr<Command> command) {
    if (commands.find(commandName) != commands.end()) {
        throw std::runtime_error("Command already registered: " + commandName);
    }
    commands[commandName] = command;
}

bool CommandInvoker::executeCommand(const std::string& commandName) {
    auto it = commands.find(commandName);
    if (it == commands.end()) {
        return false;
    }
    
    try {
        it->second->execute();
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return true;
    }
}

} // namespace cli
} // namespace password_generator