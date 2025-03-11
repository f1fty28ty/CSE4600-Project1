# MyShell

## Overview
MyShell is a simple custom shell implementation that supports built-in commands, aliasing, and the ability to execute standard Unix commands. It is designed to provide a basic interactive shell experience while allowing users to create and manage command aliases.

## Features
- Execute standard Unix commands
- Built-in command support
- Alias management (set, remove, list, save, load aliases)
- Customizable shell name and prompt terminator

## Built-in Commands

| Command                 | Description |
|-------------------------|-------------|
| `STOP`                 | Exit the shell |
| `HELP`                 | Display help menu |
| `SETSHELLNAME <name>`  | Change the shell prompt name |
| `SETTERMINATOR <symbol>` | Change the shell prompt terminator |
| `NEWNAME <alias> <command>` | Create an alias for a command |
| `NEWNAME <alias>`      | Remove an alias |
| `LISTNEWNAMES`         | List all command aliases |
| `SAVENEWNAMES <file>`  | Save aliases to a file |
| `READNEWNAMES <file>`  | Load aliases from a file |

## Installation
To compile MyShell, use the following command:

```sh
 g++ -o myshell myshell.cpp
```

Then, run the shell with:

```sh
 ./myshell
```

## Usage
Upon launching, the shell will display a prompt indicating the current user and working directory. Commands can be entered directly. Built-in commands can be used to customize the shell behavior, and Unix commands can be executed normally.

Example:
```sh
 SETSHELLNAME customShell
 SETTERMINATOR $
 NEWNAME ll ls -l
 ll
```

This will change the shell prompt name, update the terminator, and create an alias for `ls -l` as `ll`.

## Alias Management
Aliases can be used to simplify command execution. For example:

```sh
 NEWNAME myls ls -al
 myls
```

Aliases persist only during the session unless saved and reloaded using `SAVENEWNAMES` and `READNEWNAMES`.

## Prompt Customization
- The shell prompt displays: `username@myshell [current_directory]`.
- It can be customized using `SETSHELLNAME` and `SETTERMINATOR`.

## Exiting MyShell
To exit MyShell, use the `STOP` command.

## License
This project is provided as-is for educational purposes.

## Author
Miles Silveria

