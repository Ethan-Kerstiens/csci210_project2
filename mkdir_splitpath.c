#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

// External references to root and current working directory (cwd)
extern struct NODE* root;
extern struct NODE* cwd;


struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Find the last '/' to separate dirName and baseName
    char* lastSlash = strrchr(pathName, '/');
    
    if (lastSlash == NULL) {
        printf("SPLITPATH ERROR: Invalid path %s\n", pathName);
        return NULL;
    }

    // Separate dirName and baseName
    strncpy(dirName, pathName, lastSlash - pathName);
    dirName[lastSlash - pathName] = '\0';
    strcpy(baseName, lastSlash + 1);

    // Handle root directory as the parent if dirName is empty
    if (strcmp(dirName, "") == 0) {
        return root;
    }

    // Traverse the directory tree to find the parent directory
    struct NODE* temp = root;
    char* token = strtok(dirName, "/");
    
    while (token != NULL) {
        struct NODE* child = temp->childPtr;
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                temp = child;
                break;
            }
            child = child->siblingPtr;
        }
        if (child == NULL) {
            printf("SPLITPATH ERROR: Directory %s not found\n", token);
            return NULL;
        }
        token = strtok(NULL, "/");
    }
    return temp;
}

/**
 * Creates a new directory at the given path.
 * If the directory already exists or the path is invalid, it prints an error.
 */
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        return; // Error message already printed by splitPath
    }

    // Check if the directory already exists
    struct NODE* existing = parent->childPtr;
    while (existing != NULL) {
        if (strcmp(existing->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        existing = existing->siblingPtr;
    }

    // Create a new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    // Insert the new directory into the parent's child list
    if (parent->childPtr == NULL) {
        parent->childPtr = newDir;
    } else {
        struct NODE* sibling = parent->childPtr;
        while (sibling->siblingPtr != NULL) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: directory %s created\n", pathName);
}
