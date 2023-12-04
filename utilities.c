#include "utilities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Function to copy a string
char* copyString(char* targetString) {
    // Calculate the length of the input string
    size_t length = strlen(targetString) + 1; // +1 for the null terminator
    
    // Allocate memory for the new string copy
    char *targetString_copy = malloc(sizeof(char) * length);
    if (targetString_copy == NULL) {
        perror("memory allocation error");
        exit(EXIT_FAILURE);
    }
    
    // Copy the string
    strcpy(targetString_copy, targetString);
    
    // Return the copied string
    return targetString_copy;
}

/*
// Main function for testing the copyString function
int main() {
    // Example usage of copyString function
    char original[] = "Hello, World!";
    char* copied = copyString(original);
    
    // Display the copied string
    printf("Original: %s\n", original);
    printf("Copied: %s\n", copied);
    
    // Clean up the allocated memory
    free(copied);
    
    return 0;
}
*/