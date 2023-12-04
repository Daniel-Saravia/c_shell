#include "command_executor.h"
#include "utilities.h"
#include "constants.h"
#include "vmm.h" // Include the VMM header

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// VMM commands
#define CMD_CREATE_PROCESS "createproc"
#define CMD_ALLOCATE_MEMORY "allocmem"
#define CMD_ACCESS_MEMORY "accessmem"
#define CMD_FREE_MEMORY "freemem"

//Modified commands
#define CMD_DELETE_DIR_EMPTY "rmdir"
#define CMD_CHANGE_DIR "cd"
#define CMD_WRITE_LINE "writeline"
#define CMD_RANDOM_WRITE "randomtxt"

// Utility function to check if the command is a VMM command
int isVMMCommand(const char *command) {
    return strcmp(command, CMD_CREATE_PROCESS) == 0 ||
           strcmp(command, CMD_ALLOCATE_MEMORY) == 0 ||
           strcmp(command, CMD_ACCESS_MEMORY) == 0 ||
           strcmp(command, CMD_FREE_MEMORY) == 0;
}

// Function to execute VMM commands
void executeVMMCommand(char **arguments) {
    if (strcmp(arguments[0], CMD_CREATE_PROCESS) == 0) {
        if (arguments[1] == NULL || arguments[2] == NULL) {
            printf("Usage: %s <pid> <memory_size>\n", CMD_CREATE_PROCESS);
            return;
        }
        unsigned int pid = (unsigned int)atoi(arguments[1]);
        size_t memory_size = (size_t)atoi(arguments[2]);
        PCB pcb;
        createProcess(&pcb, pid, memory_size);
    } else if (strcmp(arguments[0], CMD_ALLOCATE_MEMORY) == 0) {
        // Handle memory allocation command
        // Add the logic for allocating memory to a process
    } else if (strcmp(arguments[0], CMD_ACCESS_MEMORY) == 0) {
        // Handle memory access command
        // Add the logic for accessing a process's memory
    } else if (strcmp(arguments[0], CMD_FREE_MEMORY) == 0) {
        // Handle memory free command
        // Add the logic for freeing a process's memory
    } else {
        printf("Unknown VMM command.\n");
    }
}

int isModifiedCommand(const char *command) {
    return strcmp(command, CMD_DELETE_DIR_EMPTY) == 0 ||
           strcmp(command, CMD_CHANGE_DIR) == 0 ||
           strcmp(command, CMD_WRITE_LINE) == 0 ||
           strcmp(command, CMD_RANDOM_WRITE) == 0;
}

// Execute modified UNIX command. Modified terminal commands share similar functionality/syntax to their UNIX counterparts, but cannot be executed directly from
// an exec system call or may be otherwise confusing for inexperienced users.
void executeModifiedCommand(char **arguments)
{
    
    if (strcmp(arguments[0], CMD_DELETE_DIR_EMPTY) == 0) //rmdir -r [dir_name]
    {
        int errorState = rmdir(arguments[1]);
        //printf("%d\n",errno);

        if (errorState != 0) //File does not exist
        {
            printf("rmdir: ");
            if (errno == 14) //file not found
            {
                printf("Directory not found! Please review the syntax: `rmdir [dir_name]`\n");
            }
            else if (errno == 20) //attempted non-empty directory deletion
            {
                printf("File is not a directory!\n");
            }
            else if (errno == 39) //attempted non-empty directory deletion
            {
                printf("Directory not empty! To delete non-empty directories, use `rm -r [dir_name]`\n");
            }
            else
            {
                printf("An unknown error occurred.\n");
            }
        }
    }
    else if (strcmp(arguments[0], CMD_CHANGE_DIR) == 0) //rm -r [dir_name]
    {
        int errorState = chdir(arguments[1]);
        //printf("%d\n",errno);
        if (errorState != 0) //File does not exist
        {
            printf("cd: ");
            if (errno == 14) //directory not found
            {
                printf("Directory not found! Please review the syntax: `cd [dir_name]`\n");
            }
            else if (errno == 20) //attempted non-empty directory deletion
            {
                printf("File is not a directory!\n");
            }
            else
            {
                printf("rmdir: An unknown error occurred.\n");
            }
        }
    }
    else if (strcmp(arguments[0], CMD_WRITE_LINE) == 0)
    {
        FILE* writeFile = fopen(arguments[1],"a");
        for (int i = 2; arguments[i] != NULL; i++)
        {
            //printf("%s\n", arguments[i]);
            fprintf(writeFile, "%s\n", arguments[i]);
        }
        fclose(writeFile);
    }
    else if (strcmp(arguments[0], CMD_RANDOM_WRITE) == 0)
    {
        int numBytes = atoi(arguments[2]);
        char *data = malloc(sizeof(char) * (numBytes));
        for (int i = 0; i < numBytes; i++)
        {
            data[i] = (random() % 256);
            printf("%s\n",data);
        }
        printf("Checkpoint!\n");
        FILE* writeFile = fopen(arguments[1],"a");
        fprintf(writeFile, "%s", data);
        fclose(writeFile);
    }
}

// Function to create a process and execute a command
void createCommandProcess(char **arguments) {
    if (isVMMCommand(arguments[0])) {
        executeVMMCommand(arguments);
    } else if (isModifiedCommand(arguments[0])) {
        printf("Modified command executed!\n");
        executeModifiedCommand(arguments);
    } else {
        pid_t processID = fork();
        if (processID > 0) {
            int status = 0;
            waitpid(processID, &status, 0);
        } else if (processID == 0) {
            execvp(arguments[0], arguments);
            _exit(EXIT_FAILURE); // If execvp fails.
        } else {
            perror("fork failed");
            _exit(EXIT_FAILURE); // If fork fails.
        }
    }
}

// Function to create a process for executing a script file
void createFileProcess(char **arguments, char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file) {
        fclose(file);

        pid_t processID = fork();
        char **execArgv = prepareArguments(arguments);
        if (processID > 0) {
            // Parent process
        } else if (processID == 0) {
            execv("./file-handler", execArgv);
            _exit(EXIT_FAILURE); // If execv fails.
        } else {
            perror("fork failed");
            _exit(EXIT_FAILURE); // If fork fails.
        }

        int status;
        waitpid(processID, &status, 0);
    } else {
        perror("argument file does not exist");
        exit(EXIT_FAILURE);
    }
}

// Function to prepare arguments for script file execution
char **prepareArguments(char **arguments) {
    int numArguments = 0;
    for (numArguments = 0; arguments[numArguments] != NULL; numArguments++);

    char **returnArgs = malloc(sizeof(char*) * (numArguments + 2)); // for the process name and null terminator
    returnArgs[0] = "./file-handler"; // Hard-coded, but the first element of argv should be the name of the process
    for (int i = 1; i <= numArguments; i++) {
        returnArgs[i] = arguments[i-1];
    }
    returnArgs[numArguments + 1] = NULL; // null terminator

    return returnArgs;
}

