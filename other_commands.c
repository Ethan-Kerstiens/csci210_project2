
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

/**
 * Lists the contents of the given directory.
 */
void ls(char pathName[]) {
    printf("LS: Listing contents of %s\n", pathName);
    // Implement logic for listing directory contents.
}

/**
 * Changes the current working directory.
 */
void cd(char pathName[]) {
    printf("CD: Changing directory to %s\n", pathName);
    // Implement logic for changing directory.
}

/**
 * Prints the current working directory path.
 */
void pwd() {
    printf("PWD: Current working directory\n");
    // Implement logic to print the absolute path from root to cwd.
}

/**
 * Creates a new file in the specified directory.
 */
void touch(char pathName[]) {
    printf("TOUCH: Creating file %s\n", pathName);
    // Implement logic to create a new file.
}

/**
 * Removes a file or directory at the specified path.
 */
void rm(char pathName[]) {
    printf("RM: Removing %s\n", pathName);
    // Implement logic for removing files/directories.
}

/**
 * Removes an empty directory at the given path.
 */
void rmdir(char pathName[]) {
    printf("RMDIR: Removing directory %s\n", pathName);
    // Implement logic to remove an empty directory.
}
