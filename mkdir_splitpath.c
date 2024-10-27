
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        return; // Error already printed by splitPath
    }

    // This will check for the prior existance of the directory
    struct NODE* existing = parent->childPtr;
    while (existing != NULL) {
        if (strcmp(existing->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        existing = existing->siblingPtr;
    }

    // This creates the new Node for the directory
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    // This will insert this directory into the parents child
    if (parent->childPtr == NULL) {
        parent->childPtr = newDir;
    } else {
        struct NODE* sibling = parent->childPtr;
        while (sibling->siblingPtr != NULL) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}
