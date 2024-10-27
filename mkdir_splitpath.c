#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

/**
 * Splits the given `pathName` into `dirName` (directory) and `baseName` (final element).
 * Supports both absolute and relative paths.
 */
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (pathName[0] == '/') {
        strcpy(dirName, "/");
    } else {
        strcpy(dirName, cwd->name);
    }

    // Extract the base name (last part) from the path
    char* token = strrchr(pathName, '/');
    if (token) {
        strcpy(baseName, token + 1);
        strncpy(dirName, pathName, token - pathName);  // Copy the directory part
        dirName[token - pathName] = '\0';  // Null-terminate the string
    } else {
        strcpy(baseName, pathName);
    }

    // Traverse from the root or current directory to find the parent node
    struct NODE* parent = (dirName[0] == '/') ? root : cwd;
    char temp[64];
    strcpy(temp, dirName);
    token = strtok(temp, "/");

    while (token) {
        struct NODE* child = parent->childPtr;
        while (child) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                parent = child;
                break;
            }
            child = child->siblingPtr;
        }
        if (!child) {
            printf("ERROR: Directory %s not found\n", token);
            return NULL;  // Directory does not exist
        }
        token = strtok(NULL, "/");
    }

    return parent;
}

/**
 * Creates a new directory at the specified path.
 */
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: Cannot create root directory\n");
        return;
    }

    char baseName[64], dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        printf("MKDIR ERROR: Invalid path\n");
        return;
    }

    // Check if the directory already exists
    struct NODE* existing = parent->childPtr;
    while (existing) {
        if (strcmp(existing->name, baseName) == 0) {
            printf("MKDIR ERROR: Directory %s already exists\n", pathName);
            return;
        }
        existing = existing->siblingPtr;
    }

    // Create and link the new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    if (!parent->childPtr) {
        parent->childPtr = newDir;
    } else {
        struct NODE* sibling = parent->childPtr;
        while (sibling->siblingPtr) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: Directory %s created\n", pathName);
}
