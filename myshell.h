// myshell.h - Header file for the shell
#ifndef MYSHELL_H
#define MYSHELL_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <pwd.h>

class AliasManager {
private:
    std::map<std::string, std::string> aliases;
    const size_t maxAliases = 10;
public:
    void setAlias(const std::string& alias, const std::string& command);
    void removeAlias(const std::string& alias);
    void listAliases();
    void saveAliases(const std::string& filename);
    void loadAliases(const std::string& filename);
    std::string getCommand(const std::string& alias);
};

class MyShell {
private:
    std::string shellName = "myshell";
    std::string terminator = ">";
    AliasManager aliasManager;
    void executeCommand(const std::vector<std::string>& args);
    void handleBuiltInCommands(const std::vector<std::string>& args);
    void displayHelp();
    void executeExternalCommand(const std::vector<std::string>& args);
    void displayPrompt();
public:
    void start();
};

#endif // MYSHELL_H