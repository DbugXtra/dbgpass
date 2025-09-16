#include "cli/commands/CommandFactory.h"
#include "cli/commands/HelpCommand.h"
#include "cli/commands/VersionCommand.h"
#include "cli/commands/SetLengthCommand.h"
#include "cli/commands/ConfigCommands.h"
#include "cli/commands/SetSymbolsCommand.h"
#include "cli/commands/ActionCommands.h"
#include <iostream>
#include <typeinfo>

namespace password_generator {
namespace cli {
namespace commands {

std::vector<std::unique_ptr<Command>> CommandFactory::createCommands(CommandContext& context) {
    std::vector<std::unique_ptr<Command>> commands;

    while (context.currentArgIndex < context.args.size()) {
        const std::string& arg = context.getCurrentArg();

        auto command = createCommand(arg, context);
        if (!command) {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            std::cerr << "Use --help for usage information\n";
            return {}; // Return empty vector on error
        }

        commands.push_back(std::move(command));

        // If we should exit early (like help or version), return immediately
        if (context.shouldExit) {
            return commands;
        }

        context.advance();
    }

    // If no action command was specified, add the default generate command
    if (!hasActionCommand(commands)) {
        auto defaultAction = createDefaultAction(commands);
        if (defaultAction) {
            commands.push_back(std::move(defaultAction));
        }
    }

    return commands;
}

std::unique_ptr<Command> CommandFactory::createCommand(const std::string& arg, CommandContext& context) {
    if (arg == "-h" || arg == "--help") {
        return std::make_unique<HelpCommand>();
    }
    else if (arg == "--version") {
        return std::make_unique<VersionCommand>();
    }
    else if (arg == "-g" || arg == "--generate") {
        return std::make_unique<GenerateCommand>();
    }
    else if (arg == "-b" || arg == "--batch") {
        return BatchCommand::create(context);
    }
    else if (arg == "-l" || arg == "--length") {
        return SetLengthCommand::create(context);
    }
    else if (arg == "--no-lowercase") {
        return std::make_unique<NoLowercaseCommand>();
    }
    else if (arg == "--no-uppercase") {
        return std::make_unique<NoUppercaseCommand>();
    }
    else if (arg == "--no-digits") {
        return std::make_unique<NoDigitsCommand>();
    }
    else if (arg == "--no-symbols") {
        return std::make_unique<NoSymbolsCommand>();
    }
    else if (arg == "-s" || arg == "--symbols") {
        return SetSymbolsCommand::create(context);
    }
    else if (arg == "-p" || arg == "--pronounceable") {
        return std::make_unique<PronounceableCommand>();
    }
    else if (arg == "-c" || arg == "--config") {
        return std::make_unique<ConfigShowCommand>();
    }
    else if (arg == "-v" || arg == "--validate") {
        return ValidateCommand::create(context);
    }
    else if (arg == "-q" || arg == "--quiet") {
        return std::make_unique<QuietCommand>();
    }

    return nullptr; // Unknown argument
}

std::unique_ptr<Command> CommandFactory::createDefaultAction(const std::vector<std::unique_ptr<Command>>& /* commands */) {
    // Default action is to generate a single password
    return std::make_unique<GenerateCommand>();
}

bool CommandFactory::hasActionCommand(const std::vector<std::unique_ptr<Command>>& commands) {
    for (const auto& command : commands) {
        // Check if command is an action command (not a configuration command)
        if (dynamic_cast<const GenerateCommand*>(command.get()) ||
            dynamic_cast<const BatchCommand*>(command.get()) ||
            dynamic_cast<const ValidateCommand*>(command.get()) ||
            dynamic_cast<const ConfigShowCommand*>(command.get()) ||
            dynamic_cast<const HelpCommand*>(command.get()) ||
            dynamic_cast<const VersionCommand*>(command.get())) {
            return true;
        }
    }
    return false;
}

} // namespace commands
} // namespace cli
} // namespace password_generator