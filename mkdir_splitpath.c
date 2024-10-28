#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* findChild(struct NODE* parent, const char* name) {
    for (struct NODE* current = parent->childPtr; current != NULL; current = current->siblingPtr) {
        if (strcmp(current->name, name) == 0) return current;
    }
    return NULL;
}

void splitLastSlash(char* pathName, char* dirName, char* baseName) {
    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash) {
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';
        strcpy(baseName, lastSlash + 1);
    } else {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }
}

struct NODE* traversePath(const char* dirName) {
    struct NODE* current = (dirName[0] == '/') ? root : cwd;
    if (strcmp(dirName, "") == 0 || strcmp(dirName, "/") == 0) return current;

    char pathCopy[64];
    strcpy(pathCopy, dirName);
    char* token = strtok(pathCopy, "/");

    while (token && current) {
        current = findChild(current, token);
        if (!current) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        token = strtok(NULL, "/");
    }
    return current;
}

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    splitLastSlash(pathName, dirName, baseName);
    return traversePath(dirName);
}

void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[64], baseName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (!parent) return;

    if (findChild(parent, baseName)) {
        printf("MKDIR ERROR: directory %s already exists\n", pathName);
        return;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    if (!newDir) {
        printf("MKDIR ERROR: memory allocation failed\n");
        return;
    }

    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->childPtr = newDir->siblingPtr = NULL;

    if (!parent->childPtr) {
        parent->childPtr = newDir;
    } else {
        struct NODE* lastSibling = parent->childPtr;
        while (lastSibling->siblingPtr) lastSibling = lastSibling->siblingPtr;
        lastSibling->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}
