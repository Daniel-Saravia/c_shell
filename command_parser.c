#include "command_parser.h"
#include "utilities.h"
#include <string.h>
#include <stdlib.h>

// Splits a string of commands separated by semicolons into an array of commands.
char** splitCommands(char* commands) {
    // Get the length of the commands string.
    size_t commandsLength = strlen(commands);
    if (commandsLength == 0) {
        return NULL; // Return NULL for empty command strings.
    }

    char *delim = ";"; // Delimiter for splitting commands.
    char *commands_copy = copyString(commands); // Make a copy of the commands to avoid modifying the original string.

    // Count the number of commands separated by the delimiter.
    int numTokens = 0;
    char *token = strtok(commands, delim);
    while (token != NULL) {
        numTokens++;
        token = strtok(NULL, delim);
    }
    numTokens++;
    // Allocate memory for the array of command strings.
    char **argumentsV = malloc(sizeof(char *) * (numTokens + 1));

    // Split the copied commands string into individual commands.
    token = strtok(commands_copy, delim);
    int i = 0;
    for (i = 0; token != NULL; i++) {
        argumentsV[i] = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(argumentsV[i], token); // Copy each command into the array.
        token = strtok(NULL, delim);
    }
    argumentsV[i] = NULL; // Null-terminate the array of commands.
    
    free(commands_copy); // Free the copied commands string.

    return argumentsV; // Return the array of commands.
}

// Parses a list of commands into a list of argument vectors.
char*** parseCommandList(char** commandList) {
    // Count the number of commands in the command list.
    int numCommands = 0;
    for (numCommands = 0; commandList[numCommands] != NULL; numCommands++);

    if (numCommands == 0) {
        return NULL; // Return NULL for empty command lists.
    }

    // Allocate memory for the array of argument vectors.
    char ***argList = malloc(sizeof(char**) * (numCommands + 1));
    for (int i = 0; commandList[i] != NULL; i++) {
        argList[i] = getArgumentList(commandList[i]); // Convert each command into an argument vector.
    }
    argList[numCommands] = NULL; // Null-terminate the array of argument vectors.
    
    return argList; // Return the array of argument vectors.
}

// Converts a single command string into an argument vector.
char** getArgumentList(char* command) {
    char *delim = " \n"; // Delimiters for splitting the command into arguments.
    char *command_copy = copyString(command); // Make a copy of the command.

    // Count the number of arguments in the command.
    int numTokens = 0;
    char *token = strtok(command, delim);
    while (token != NULL) {
        numTokens++;
        token = strtok(NULL, delim);
    }
    numTokens++;
    // Allocate memory for the argument vector.
    char **argumentsV = malloc(sizeof(char *) * (numTokens + 1));

    // Split the copied command string into individual arguments.
    token = strtok(command_copy, delim);
    int i = 0;
    for (i = 0; token != NULL; i++) {
        argumentsV[i] = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(argumentsV[i], token); // Copy each argument into the vector.
        token = strtok(NULL, delim);
    }
    argumentsV[i] = NULL; // Null-terminate the argument vector.
    
    free(command_copy); // Free the copied command string.

    return argumentsV; // Return the argument vector.
}
