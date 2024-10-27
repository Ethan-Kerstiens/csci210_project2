#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

// Maximum number of messages to store
#define MAX_OUTPUT 1024

// Buffer to store all output messages
char outputBuffer[MAX_OUTPUT][256];
int outputIndex = 0;

// Declare quit() as an external function (defined in main.c)
extern int quit();

// Store messages in the buffer
void storeOutput(const char* message) {
    if (outputIndex < MAX_OUTPUT) {
        strncpy(outputBuffer[outputIndex], message, 255);
        outputBuffer[outputIndex][255] = '\0';  // Ensure null termination
        outputIndex++;
    }
}

// Print all stored output messages at the end
void printAllOutputs() {
    printf("\n--- Program Output ---\n");
    for (int i = 0; i < outputIndex; i++) {
        printf("%s", outputBuffer[i]);
    }
    printf("--- End of Output ---\n");
}

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (pathName[0] == '/') {
        strcpy(dirName, "/");
    } else {
        strcpy(dirName, cwd->name);
    }

    char* token = strrchr(pathName, '/');
    if (token) {
        strcpy(baseName, token + 1);
        strncpy(dirName, pathName, token - pathName);
        dirName[token - pathName] = '\0';
    } else {
        strcpy(baseName, pathName);
    }

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
            char errorMsg[256];
            snprintf(errorMsg, sizeof(errorMsg), "ERROR: Directory %s not found\n", token);
            storeOutput(errorMsg);
            return NULL;
        }
        token = strtok(NULL, "/");
    }

    return parent;
}

void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0) {
        storeOutput("MKDIR ERROR: Cannot create root directory\n");
        return;
    }

    char baseName[64], dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        storeOutput("MKDIR ERROR: Invalid path\n");
        return;
    }

    struct NODE* existing = parent->childPtr;
    while (existing) {
        if (strcmp(existing->name, baseName) == 0) {
            char errorMsg[256];
            snprintf(errorMsg, sizeof(errorMsg), "MKDIR ERROR: Directory %s already exists\n", pathName);
            storeOutput(errorMsg);
            return;
        }
        existing = existing->siblingPtr;
    }

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

    char successMsg[256];
    snprintf(successMsg, sizeof(successMsg), "MKDIR SUCCESS: Directory %s created\n", pathName);
    storeOutput(successMsg);
}
