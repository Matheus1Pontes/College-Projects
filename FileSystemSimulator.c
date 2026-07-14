/*
Name: Matheus Pontes
Final Project: Simple File System Simulator
Disk: 100 blocks size and each one has 512 bytes
Blocks 0-9: are used for Free Map
Blocks 10-99: space for files left  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// number of blocks
#define BLOCKS 100
// size of each block
#define BLOCK_SIZE 512
// max size of name of a file
#define MAX_NAME 32
// max size of the content you can write in a file 
#define MAX_CONTENT 256

/* 
Free Map: 
0 = free 
1 = used 
*/
int freeMap[BLOCKS];

// File Table stores the filename at each block in ascending order 
char fileTable[BLOCKS][MAX_NAME];

// Disk storage, this is the actual data
char disk[BLOCKS][BLOCK_SIZE];


// Initializes Disk, Free Map, and File Table
void formatDisk() {
    printf("Forming disk...\n");
    Sleep(3000);

    // Set everything to free and clear data
    for (int i = 0; i < BLOCKS; i++) {
        freeMap[i] = 0; // Setting everything to 0, so they are free
        strcpy(fileTable[i], ""); // Deleting everything in the table
        strcpy(disk[i], ""); // Deleting everything in the disk
    }

    // Reserve blocks 0-9 for Free Map
    for (int i = 0; i < 10; i++) {
        freeMap[i] = 1; // the first 10 blocks are used
    }

    printf("Disk formed successfully. FreeMap blocks 0-9 are now allocated.\n");
}


// Returns block index of file, or -1 if not found
// Not actual command, bet helps for other commands that include <filename>
int findFile(char filename[]) {
    for (int i = 10; i < BLOCKS; i++) {
        if (freeMap[i] == 1 && strcmp(fileTable[i], filename) == 0) {
            return i;
        }
    }
    return -1;
}


void createFile(char filename[]) {
    // Check if file already exists
    if (findFile(filename) != -1) {
        printf("Error: File already exists.\n");
        return;
    }

    // Find first free block
    for (int i = 10; i < BLOCKS; i++) {
        if (freeMap[i] == 0) {
            freeMap[i] = 1;
            strcpy(fileTable[i], filename);
            printf("File '%s' created successfully at block %d.\n", filename, i);
            return;
        }
    }
    // Returns this string if filename doesn't exist and if there are no empty spaces (0) left
    printf("Error: Disk is full.\n");
}


void writeFile(char filename[]) {
    int block = findFile(filename);

    if (block == -1) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }

    // Array of char is given size because it can't be undefined 
    char content[MAX_CONTENT];

    printf("Enter content to write (max 256 bytes): ");

    // Accepting input 
    getchar(); // Clear new line
    fgets(content, MAX_CONTENT, stdin);

    // Remove new line
    content[strcspn(content, "\n")] = 0;

    strcpy(disk[block], content);

    printf("Content written to '%s'.\n", filename);
}


void readFile(char filename[]) {
    int block = findFile(filename);

    if (block == -1) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }

    printf("--- Content of '%s' ---\n", filename);
    printf("%s\n", disk[block]);
}


void deleteFile(char filename[]) {
    int block = findFile(filename);

    if (block == -1) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }

    // Deleting file at index # block
    freeMap[block] = 0;
    strcpy(fileTable[block], "");
    strcpy(disk[block], "");

    printf("File '%s' deleted successfully.\n", filename);
}


void listFiles() {
    printf("--- Files on disk ---\n");

    for (int i = 10; i < BLOCKS; i++) {
        if (freeMap[i] == 1) {
            printf("-- %s\n", fileTable[i]);
        }
    }
    printf("---------------------\n");
}


int main() {
    // Max size a command can be
    char command[50];
    char filename[MAX_NAME];

    // Initial setup
    formatDisk();

    printf("\nWelcome to the simple file system simulator!\n");
    printf("\nCommands:\n");
    printf("--format\n--create <filename>\n--read <filename>\n--write <filename>\n--delete <filename>\n--ls\n--exit\n");

    // Basically means while true
    while (1) {
        printf("\n> Please enter command: ");
        scanf("%s", command);

        if (strcmp(command, "format") == 0) {
            formatDisk();
        } else if (strcmp(command, "create") == 0) {
            // Accepting input for filename and then creating said filename
            scanf("%s", filename);
            createFile(filename);
        } else if (strcmp(command, "read") == 0) {
            // Accepting input for filename and then reading file
            scanf("%s", filename);
            readFile(filename);
        } else if (strcmp(command, "write") == 0) {
            // Accepting input for filename and then writing content in the file
            scanf("%s", filename);
            writeFile(filename);
        } else if (strcmp(command, "delete") == 0) {
            // Accepting input for filename and then deleting the file 
            scanf("%s", filename);
            deleteFile(filename);
        } else if (strcmp(command, "ls") == 0) {
            listFiles(); // Listing all filenames in directory 
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting file system simulator...\n");
            Sleep(3000);
            // Will cease the simulator 
            break;
        } else {
            // Just in case user types anything other than these given commands 
            printf("Invalid command.\n");
        }
    }
    return 0;
}