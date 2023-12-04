#include "builtin_commands.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "createFileProcess.h"

// Function to check and execute built-in commands.
int checkAdditionalCommands(char** arguments) {
    // Check if the first argument is NULL or the 'quit' command, and if so, exit the shell.
    if ((!arguments[0]) || (strcmp(arguments[0], CMD_QUIT) == 0)) {
        printf("Exiting shell...\n");
        exit(EXIT_SUCCESS); // Exit the program successfully.
    }

    // If the first argument is 'help', display the help information.
    if (strcmp(arguments[0], CMD_HELP) == 0) {
        showHelp(arguments); // Call the function to show help information.
        return 1; // Indicate that a built-in command was processed.
    }

    // If the first argument is 'execute file', execute a batch file.
    if (strcmp(arguments[0], CMD_EXECUTE_FILE) == 0) {
        printf("Executing file!\n");
        // Call the function to create and execute a file process.
        createFileProcess(arguments, arguments[1]);
        return 1; // Indicate that a built-in command was processed.
    }
    
    return 0; // Return 0 if it's a standard command and not a built-in command.
}

// Function to display help information based on the arguments provided.
void showHelp(char** arguments) {
    int helpInfo = HELP_DEFAULT; // Default help information.

    // Check if a specific help topic is requested.
    if (arguments[1] != NULL) {
        // Determine the type of help information needed based on the second argument.
        if (strcmp(arguments[1], CMD_EXECUTE_FILE) == 0) {
            helpInfo = HELP_EXECUTE_FILE;
        } else if (strcmp(arguments[1], CMD_HELP) == 0) {
            helpInfo = HELP_HELP;
        } else if (strcmp(arguments[1], CMD_QUIT) == 0) {
            helpInfo = HELP_QUIT;
        } else {
            helpInfo = HELP_ERROR; // If the command is not recognized.
        }
    }

    // Display help information based on the determined help type.
    switch (helpInfo) {
        case HELP_DEFAULT:
        default:
            // General help information for various commands.
            printf("Enter `help [commandName]` to learn more about a shell given command.\n");
            printf("- %s: Execute a bash script file.\n", CMD_EXECUTE_FILE);
            printf("- %s: Info on using lopesShell. Use `%s %s` for additional information on features.\n", CMD_HELP, CMD_HELP, CMD_HELP);
            printf("- %s: Exit lopesShell.\n", CMD_QUIT);
            break;
        case HELP_EXECUTE_FILE:
            // Help information for executing a file.
            printf("%s: Execute a bash script file.\n", CMD_EXECUTE_FILE);
            printf("Syntax: `%s [fileName]`\n", CMD_EXECUTE_FILE);
            printf("- fileName: The name of the file containing the bash script. Ideally, the file should be a .txt file.\n");
            break;
        case HELP_HELP:
            // Help information for the help command itself.
            printf("%s: Info on using lopesShell.\n", CMD_HELP);
            printf("Syntax: `%s [commandName]`\n", CMD_HELP);
            printf("- commandName: The name of a command specifically added by lopesShell.\n");
            printf("Additional Information:\n");
            printf("- When in prompt, multiple commands can be entered at once, separated by semicolons (;).\n");
            printf("- When executing from files, multiple commands can be entered at once, separated by line breaks or semicolons (;).\n");
            printf("  - NOTE: Bash script files WILL NOT run lopesShell-specific commands!\n");
            break;
        case HELP_QUIT:
            // Help information for the quit command.
            printf("%s: Exit lopesShell.\n", CMD_QUIT);
            printf("Syntax: `%s`\n", CMD_QUIT);
            printf("- No arguments required.\n");
            break;
        case HELP_ERROR:
            // Display error message for invalid command name in help request.
            printf("Error! Invalid command name.\n");
            printf("Use `%s` to view a list of valid commands.\n", CMD_HELP);
            break;
    }
}


