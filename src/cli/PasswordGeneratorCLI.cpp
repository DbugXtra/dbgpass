#include "cli/PasswordGeneratorCLI.h"
#include "cli/commands/CommandContext.h"
#include "cli/commands/CommandFactory.h"
#include "cli/commands/CommandInvoker.h"
#include <iostream>
#include <vector>
#include <memory>

namespace password_generator {
namespace cli {

class PasswordGeneratorCLI::Impl {
public:
    int processArgs(int argc, char* argv[]) {
        using namespace commands;

        // Create command context
        CommandContext context(argv[0]);

        // Convert argv to vector of strings (skip program name)
        for (int i = 1; i < argc; ++i) {
            context.args.emplace_back(argv[i]);
        }

        // Handle empty arguments
        if (context.args.empty()) {
            context.showUsage();
            return 1; // No args provided, show usage
        }

        // Create commands using factory
        auto commands = CommandFactory::createCommands(context);
        if (commands.empty()) {
            return 1; // Error occurred during command creation
        }

        // Execute commands using invoker
        CommandInvoker invoker;
        for (auto& command : commands) {
            invoker.addCommand(std::move(command));
        }

        return invoker.executeCommands(context);
    }
};

PasswordGeneratorCLI::PasswordGeneratorCLI()
    : pImpl(std::make_unique<Impl>()) {}

PasswordGeneratorCLI::~PasswordGeneratorCLI() = default;

int PasswordGeneratorCLI::processArgs(int argc, char* argv[]) {
    return pImpl->processArgs(argc, argv);
}


} // namespace cli
} // namespace password_generator