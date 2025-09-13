#ifndef PASSWORD_GENERATOR_CLI_H
#define PASSWORD_GENERATOR_CLI_H

#include <memory>
#include <string>

namespace password_generator {
namespace cli {

/**
 * @brief Command-line interface for password generator
 */
class PasswordGeneratorCLI {
public:
    PasswordGeneratorCLI();
    ~PasswordGeneratorCLI();
    
    /**
     * @brief Run the interactive CLI
     */
    void run();
    
    /**
     * @brief Process command-line arguments for automated access
     */
    int processArgs(int argc, char* argv[]);
    
    /**
     * @brief Process a single command
     */
    bool processCommand(const std::string& command);
    
    /**
     * @brief Set quiet mode (no prompts)
     */
    void setQuietMode(bool quiet);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace cli
} // namespace password_generator

#endif // PASSWORD_GENERATOR_CLI_H