/*
 * Name: MyShell
 * ID: [Your ID Here]
 * Date: [Current Date]
 * Purpose: This program implements a custom shell with built-in commands, aliasing,
 *          and the ability to execute standard Unix commands.
 */

#include "myshell.h"
#include <pwd.h>

/* ================================ */
/*         AliasManager Class        */
/* ================================ */

/*
 * Purpose: Sets an alias for a command.
 * Parameters:
 *   - alias: The alias name to be created.
 *   - command: The actual command that the alias will represent.
 * Return: None.
 */
void AliasManager::setAlias(const std::string &alias, const std::string &command) {
    if (aliases.size() >= maxAliases) {
        std::cerr << "Alias limit reached. Cannot add more aliases." << std::endl;
        return;
    }
    aliases[alias] = command;
}

/*
 * Purpose: Removes an alias if it exists.
 * Parameters:
 *   - alias: The alias name to be removed.
 * Return: None.
 */
void AliasManager::removeAlias(const std::string &alias) {
    if (aliases.erase(alias) == 0) {
        std::cerr << "Alias not found." << std::endl;
    }
}

/*
 * Purpose: Lists all defined aliases.
 * Parameters: None.
 * Return: None.
 */
void AliasManager::listAliases() {
    for (const auto &pair : aliases) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }
}

/*
 * Purpose: Saves all aliases to a file.
 * Parameters:
 *   - filename: The file where aliases will be saved.
 * Return: None.
 */
void AliasManager::saveAliases(const std::string &filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for saving aliases." << std::endl;
        return;
    }
    for (const auto &pair : aliases) {
        file << pair.first << " " << pair.second << std::endl;
    }
    file.close();
}

/*
 * Purpose: Loads aliases from a file.
 * Parameters:
 *   - filename: The file from which aliases will be loaded.
 * Return: None.
 */
void AliasManager::loadAliases(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file '" << filename << "' for reading aliases." << std::endl;
        return;
    }
    aliases.clear();
    std::string alias, command;
    while (file >> alias >> command) {
        aliases[alias] = command;
    }
    std::cout << "Aliases successfully loaded from " << filename << "." << std::endl;
    file.close();
}

/*
 * Purpose: Retrieves the command associated with an alias.
 * Parameters:
 *   - alias: The alias whose command needs to be retrieved.
 * Return: The actual command if found, otherwise returns the alias itself.
 */
std::string AliasManager::getCommand(const std::string &alias) {
    return aliases.count(alias) ? aliases[alias] : alias;
}

/* ================================ */
/*          MyShell Class           */
/* ================================ */

/*
 * Purpose: Executes an external command, replacing aliases if necessary.
 * Parameters:
 *   - args: The vector containing command and arguments.
 * Return: None.
 */
void MyShell::executeExternalCommand(const std::vector<std::string> &args) {
    if (args.empty()) return;

    std::string command = aliasManager.getCommand(args[0]);

    if (fork() == 0) {
        std::vector<char *> execArgs;
        execArgs.push_back(const_cast<char *>(command.c_str()));
        for (size_t i = 1; i < args.size(); i++) {
            execArgs.push_back(const_cast<char *>(args[i].c_str()));
        }
        execArgs.push_back(nullptr);
        execvp(execArgs[0], execArgs.data());

        if (errno == ENOENT) {
            std::cerr << "Error: Command '" << command << "' not found." << std::endl;
        } else {
            std::cerr << "Error: Incorrect usage of '" << command << "'. Try '" << command << " --help' for correct syntax." << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    wait(nullptr);
}

/*
 * Purpose: Handles built-in shell commands and executes appropriate actions.
 * Parameters:
 *   - args: A vector containing the parsed command and its arguments.
 * Return: None.
 */
void MyShell::handleBuiltInCommands(const std::vector<std::string>& args) {
    if (args.empty()) return;

    if (args[0] == "STOP") {
        exit(0);
    } else if (args[0] == "HELP") {
        displayHelp();
    } else if (args[0] == "SETSHELLNAME") {
        if (args.size() != 2) {
            std::cerr << "Usage: SETSHELLNAME <name>" << std::endl;
            return;
        }
        shellName = args[1];
    } else if (args[0] == "SETTERMINATOR") {
        if (args.size() != 2) {
            std::cerr << "Usage: SETTERMINATOR <symbol>" << std::endl;
            return;
        }
        terminator = args[1];
    } else if (args[0] == "NEWNAME") {
        if (args.size() == 3) {
            aliasManager.setAlias(args[1], args[2]);
        } else if (args.size() == 2) {
            aliasManager.removeAlias(args[1]);
        } else {
            std::cerr << "Usage: NEWNAME <alias> <command> or NEWNAME <alias> to remove" << std::endl;
        }
    } else if (args[0] == "LISTNEWNAMES") {
        aliasManager.listAliases();
    } else if (args[0] == "SAVENEWNAMES") {
        if (args.size() != 2) {
            std::cerr << "Usage: SAVENEWNAMES <file>" << std::endl;
            return;
        }
        aliasManager.saveAliases(args[1]);
    } else if (args[0] == "READNEWNAMES") {
        if (args.size() != 2) {
            std::cerr << "Usage: READNEWNAMES <file>" << std::endl;
            return;
        }
        aliasManager.loadAliases(args[1]);
    } else {
        executeExternalCommand(args);
    }
}

/*
 * Purpose: Displays the help menu for built-in commands.
 * Parameters: None.
 * Return: None.
 */
void MyShell::displayHelp() {
    std::cout << "Available Commands:\n";
    std::cout << "STOP - Exit the shell\n";
    std::cout << "SETSHELLNAME <name> - Change the shell prompt name\n";
    std::cout << "SETTERMINATOR <symbol> - Change the shell prompt terminator\n";
    std::cout << "NEWNAME <alias> <command> - Create an alias for a command\n";
    std::cout << "LISTNEWNAMES - List all command aliases\n";
    std::cout << "SAVENEWNAMES <file> - Save aliases to a file\n";
    std::cout << "READNEWNAMES <file> - Load aliases from a file\n";
    std::cout << "<Unix command> - Execute any valid Unix command\n";
    std::cout << "Tip: Start by setting aliases with NEWNAME or customizing the shell prompt with SETSHELLNAME." << std::endl;
}

/*
 * Purpose: Displays the shell prompt, including username, shell name, and working directory.
 * Parameters: None.
 * Return: None.
 */
void MyShell::displayPrompt() {
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    struct passwd *pw = getpwuid(getuid());
    std::string username = pw->pw_name;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string homeDir = pw->pw_dir;
    std::string cwdStr = cwd;
    if (cwdStr.find(homeDir) == 0) {
        cwdStr.replace(0, homeDir.length(), "~");
    }
    std::cout << "\033[1;32m" << username << "@" << shellName << " \033[1;34m" << cwdStr << "\n"
              << "\033[1;36m╰─" << terminator << " \033[0m";
}

/*
 * Purpose: Starts the shell loop, reading and executing commands.
 * Parameters: None.
 * Return: None.
 */
void MyShell::start() {
    shellName = "myshell";
    terminator = ">";
    std::string input;
    while (true) {
        displayPrompt();
        std::getline(std::cin, input);
        if (input.empty()) continue;
        std::vector<std::string> args;
        size_t pos = 0;
        while ((pos = input.find(' ')) != std::string::npos) {
            args.push_back(input.substr(0, pos));
            input.erase(0, pos + 1);
        }
        args.push_back(input);
        handleBuiltInCommands(args);
    }
}
