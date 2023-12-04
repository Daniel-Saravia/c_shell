
# UNIX-SWE-350-Shell
README.TXT
===========

lopesShell: A Simple Shell with Virtual Memory Management (VMM)
===============================================================

lopesShell is a custom Unix shell that provides basic command execution along with a simulated Virtual Memory Manager (VMM). This shell allows you to execute standard Unix commands as well as special commands that simulate the allocation and management of virtual memory for processes.

Getting Started
---------------
To compile lopesShell, navigate to the directory containing the source code and run the following command in your terminal:

    gcc -o lopesShell main.c command_parser.c command_executor.c builtin_commands.c utilities.c vmm.c scheduler.c -I.

This will generate an executable named 'lopesShell'. To start the shell, run:

    ./lopesShell

Once the shell is running, you will be prompted with the shell name followed by a colon and a space, indicating that it is waiting for input.

Shell Commands
--------------
The following is a list of commands that lopesShell accepts:

- `exec [filename]`: Execute a script from the specified file.
- `help [command]`: Display help information for the given command.
- `quit`: Exit the shell.

VMM Commands
------------
lopesShell includes a set of commands to simulate virtual memory management:

- `createproc <pid> <memory_size>`: Create a new process with a specified PID and memory size requirement.
- `allocmem <pid> <size>`: Allocate additional memory to an existing process.
- `accessmem <pid> <virtual_address>`: Access a memory address within a process's virtual memory space.
- `freemem <pid> <size>`: Free a block of memory from a process.

Example Usage
-------------
Here is an example of how to use VMM commands in lopesShell:

    $lopesShell: createproc 1 1000
    $lopesShell: allocmem 1 500
    $lopesShell: accessmem 1 400
    $lopesShell: freemem 1 300
    $lopesShell: quit

Please replace '1', '1000', '500', '400', and '300' with the appropriate process ID and memory sizes for your simulation.

File System Management
----------------------
The following Linux terminal commands are confirmed to execute LopesShell with the following sytax:

- `mkdir [dir_name]` - create new directory
- `mv [current_path] [new_path]` - move/rename a file or directory
    - In order to rename a file, simply declare the same current and new directories, but with a different file name at the end of the path.
 - `rmdir [dir_name]` - delete empty directory
    - If a non-empty directory is declared, the user is prompted to use `rm -r [dir_name]` instead
 - `rm -r [dir_name]` - delete non-empty directory
 - `touch [filename]` - create empty text file
- `rm [filename]` - delete a file
- `cp [filename] [copy_filename]` - copy a file
- `cp [filename] [copy_filename] -r` - copy a directory
- `find . -name [filename]` - search for a file in a directory tree
- `du -h [directory (optional)]` - display a directory tree from a given starting node
    - If no starting node is given, displays a directory tree from the current folder.
- `ls -l [filename]` - Get basic information about a file
- `stat [filename]` - Get detailed information about a file
- `ld -ld [dir_name]` - Get basic information about a directory
- `stat [dir_name]` - Get detailed information about a directory
- `cd [path]` - Change active directory

The following supplemental commands are added by lopesShell:

- `writeline [filename] [args] ...` - Append lines to a file. Each argument after `[filename]` is written to a separate line in the designated file.
  - LopesShell is incapable of piping. Thus, traditional solutions for editing a text file such as `echo [string] >> [filename]` are non-functional.
- `randomtxt [filename] [numChars]` - Create a text file with `[numChars]` random characters.

Thank you for using lopesShell!

