#include "command_parser.h"
#include "command_executor.h"
#include "builtin_commands.h"
#include "utilities.h"
#include "constants.h"
#include "runCommand.h"
#include "vmm.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define CMD_CREATE_PROCESS "createproc"
#define CMD_ALLOCATE_MEMORY "allocmem"
#define CMD_ACCESS_MEMORY "accessmem"
#define CMD_FREE_MEMORY "freemem"

/*  Project 6
*   Most commands are covered by execution of UNIX shell commands
*
*   - Create and rename directories.
*       - Create: `mkdir [dir_name]`
*       - Rename: `mv [current_name] [new_name]`
*           new_name must be in the same directory as current_name
*   Delete directories, with special handling of non-empty directories (e.g., special flag to delete command).
*       - Delete empty: `rmdir [dir_name]`
*       - Delete non-empty: `rm -r [dir_name]`
*       *IF RMDIR IS CALLED ON A NON-EMPTY DIRECTORY, TELL THE USER TO USE NON-EMPTY SYNTAX
*   Create, rename, edit, and delete files.
*       - Create: `touch [filename]`
*       - Rename: `mv [current_name] [new_name]`
*       - Edit: `writeline [filename] [arg1] [arg2] ...`
*            Echo pipeline is difficult to implement (`echo [string] >> [filename]`), using custom command
*            Should we add functionality for removing text from a file? (e.g. search and delete strings)
*       - Delete: `rm [filename]`
*   - When a file is created, a special flag will indicate the amount of data (bytes) to (randomly) generate for it (in order to eliminate the need to open and edit each file).
*       - Create Random Text File: randomtxt [filename] [bytes]
*   - Move files across directories.
*       - `mv [current_path] [new_path]`
*           Name of file can be changed during the move.
*   Duplicate files.
*       - `cp [filename] [copy_filename]`
*       *IF CALLED ON A DIRECTORY, TELL USER TO USE DIRECTORY COPY COMMAND
*   - Duplicate directories.
*       - `cp [filename] [copy_filename] -r`
*   - Search for a file in a directory tree.
*       - `find . -name [filename]`
*   - Display a directory tree given a starting node.
*       - `du -h [directory (optional)]`
*   - Get basic information about a file.
*       - `ls -l [filename]`
*   - Get detailed information about a file (e.g., using a special flag).
*       - `stat [filename]`
*   - Get basic information about a directory.
*       - `ld -ld [dir_name]`
*   - Get detailed information about a directory (e.g., using a flag).
*       - `stat [dir_name]`
*   - Change active directory
*       - `cd [path]`
*/

// Function prototypes
void runCommand(char* inputCommand);
void handleCreateProcessCommand(char** arguments);

int main(int argc, char **argv) {
    // Initialize the virtual memory manager and the scheduler for the shell.
    initializeVMM();
    initialize_scheduler();

    // Check if any arguments (like a filename) were passed to the shell at launch.
    if (argc > 1) {
        // Execute commands from a file if a filename is provided.
        createFileProcess(argv, argv[1]);
    }

    // Main loop of the shell: continuously prompt for and process commands.
    while (true) {
        char *inputCommand = NULL;
        size_t commandSize = 0;
        ssize_t charsRead = 0;

        // Display the shell prompt and read a line of input from the user.
        printf("%s", SHELL_NAME);
        charsRead = getline(&inputCommand, &commandSize, stdin);

        // If the input is valid (not EOF), process the command.
        if (charsRead > 0) {
            runCommand(inputCommand);
        }

        // Free the memory allocated for the input command.
        free(inputCommand);

        // Execute the scheduler to manage processes.
        execute_scheduler();
    }
    // Return 0 from main (though in this case, the loop is infinite).
    return 0;
}

// Function to process and execute a given command.
void runCommand(char* inputCommand) {
    // Split the input command into individual commands (if multiple commands are separated by semicolons).
    char** commandList = splitCommands(inputCommand);
    // Parse each command in the list into an array of its arguments.
    char*** arguments = parseCommandList(commandList);

    // Iterate through each command in the command list.
    for (int i = 0; arguments && arguments[i] != NULL; i++) {
        // Check if the command is to create a process.
        if (strcmp(arguments[i][0], CMD_CREATE_PROCESS) == 0) {
            handleCreateProcessCommand(arguments[i]);
            continue;
        }

        // Check if the command is a built-in command and execute it if it is.
        if (checkAdditionalCommands(arguments[i]) == 1) {
            continue; // Skip to the next command if it was a built-in command.
        } else {
            // If not a built-in command, create a process to execute it.
            createCommandProcess(arguments[i]);
        }
    }

    // Clean up: free the memory allocated for the command list and its arguments.
    for (int i = 0; commandList && commandList[i] != NULL; i++) {
        free(commandList[i]);
    }
    free(commandList);

    for (int i = 0; arguments && arguments[i] != NULL; i++) {
        for (int j = 0; arguments[i][j] != NULL; j++) {
            free(arguments[i][j]);
        }
        free(arguments[i]);
    }
    free(arguments);
}

// Function to handle the creation of a new process.
void handleCreateProcessCommand(char** arguments) {
    // Extract process details from arguments and create a new process
    // For simplicity, assuming the process ID and burst time are provided as arguments
    if (arguments[1] && arguments[2]) {
        int process_id = atoi(arguments[1]);
        int burst_time = atoi(arguments[2]);

        Process new_process = {process_id, READY, burst_time};
        if (!add_process(new_process)) {
            printf("Failed to add process: %d\n", process_id);
        }
    }
}


