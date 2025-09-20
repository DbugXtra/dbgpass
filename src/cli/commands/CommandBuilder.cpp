#include "cli/commands/CommandBuilder.h"
#include "cli/commands/HelpCommand.h"
#include "cli/commands/VersionCommand.h"
#include "cli/commands/SetLengthCommand.h"
#include "cli/commands/ConfigCommands.h"
#include "cli/commands/SetSymbolsCommand.h"
#include "cli/commands/ActionCommands.h"

namespace password_generator {
namespace cli {
namespace commands {

CommandBuilder& CommandBuilder::getInstance() {
    static CommandBuilder instance;
    return instance;
}

CommandBuilder::CommandBuilder() {
    initializeRegistry();
}

CommandBuilder& CommandBuilder::registerCommand(const std::vector<std::string>& args, CommandCreator creator) {
    for (const auto& arg : args) {
        commandRegistry[arg] = creator;
    }
    return *this;
}

std::unique_ptr<Command> CommandBuilder::createCommand(const std::string& arg, CommandContext& context) const {
    auto it = commandRegistry.find(arg);
    if (it != commandRegistry.end()) {
        return it->second(context);
    }
    return nullptr;
}

void CommandBuilder::initializeRegistry() {
    // Help and version commands
    registerCommand({"-h", "--help"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<HelpCommand>();
        });

    registerCommand({"--version"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<VersionCommand>();
        });

    // Action commands
    registerCommand({"-g", "--generate"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<GenerateCommand>();
        });

    registerCommand({"-b", "--batch"},
        [](CommandContext& context) -> std::unique_ptr<Command> {
            return BatchCommand::create(context);
        });

    registerCommand({"-v", "--validate"},
        [](CommandContext& context) -> std::unique_ptr<Command> {
            return ValidateCommand::create(context);
        });

    registerCommand({"-c", "--config"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<ConfigShowCommand>();
        });

    // Configuration commands
    registerCommand({"-l", "--length"},
        [](CommandContext& context) -> std::unique_ptr<Command> {
            return SetLengthCommand::create(context);
        });

    registerCommand({"--no-lowercase"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<NoLowercaseCommand>();
        });

    registerCommand({"--no-uppercase"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<NoUppercaseCommand>();
        });

    registerCommand({"--no-digits"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<NoDigitsCommand>();
        });

    registerCommand({"--no-symbols"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<NoSymbolsCommand>();
        });

    registerCommand({"-s", "--symbols"},
        [](CommandContext& context) -> std::unique_ptr<Command> {
            return SetSymbolsCommand::create(context);
        });

    registerCommand({"-p", "--pronounceable"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<PronounceableCommand>();
        });

    registerCommand({"-q", "--quiet"},
        [](CommandContext&) -> std::unique_ptr<Command> {
            return std::make_unique<QuietCommand>();
        });
}

} // namespace commands
} // namespace cli
} // namespace password_generator