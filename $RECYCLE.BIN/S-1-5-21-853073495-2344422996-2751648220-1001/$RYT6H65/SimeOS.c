#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Maximum number of processes and total memory size
#define MAX_PROCESSES 10
#define MEMORY_SIZE 1024

// Structure to store process information
typedef struct {
    int id; // Process ID
    char name[20]; // Process name
    int memory_used; // Memory used by the process
} Process;

Process processes[MAX_PROCESSES]; // Array of processes
int process_count = 0; // Current number of processes
int memory_used = 0; // Total memory used

// Function to display text
void display(const char *text) {
    printf("%s\n", text);
}

// Create a process
void create_process(char *name, int memory) {
    if (process_count >= MAX_PROCESSES) { // Check if max processes limit is reached
        display("Error: Maximum number of processes reached.");
        return;
    }
    if (memory_used + memory > MEMORY_SIZE) { // Check for sufficient memory
        display("Error: Not enough memory.");
        return;
    }
    // Create a new process
    processes[process_count].id = process_count + 1;
    strcpy(processes[process_count].name, name);
    processes[process_count].memory_used = memory;
    process_count++;
    memory_used += memory;
    display("Process created.");
}

// Delete a process
void delete_process(int id) {
    int found = 0; // Flag to check if the process exists
    for (int i = 0; i < process_count; i++) {
        if (processes[i].id == id) {
            // Free memory and shift processes
            memory_used -= processes[i].memory_used;
            for (int j = i; j < process_count - 1; j++) {
                processes[j] = processes[j + 1];
            }
            process_count--;
            found = 1;
            display("Process deleted.");
            break;
        }
    }
    if (!found) display("Error: Process not found.");
}

// Display the list of processes
void list_processes() {
    printf("Process list:\n");
    for (int i = 0; i < process_count; i++) {
        printf("ID: %d, Name: %s, Memory Used: %d\n",
               processes[i].id, processes[i].name, processes[i].memory_used);
    }
    printf("Total memory used: %d/%d\n", memory_used, MEMORY_SIZE);
}

// Create a file
void create_file(const char *filename) {
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (file) {
        fclose(file);
        printf("File '%s' created successfully.\n", filename);
    } else {
        printf("Error: Failed to create file '%s'.\n", filename);
    }
}

// Delete a file
void delete_file(const char *filename) {
    if (unlink(filename) == 0) { // Delete the file
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error: Failed to delete file '%s'.\n", filename);
    }
}

// Edit a file
void edit_file(const char *filename) {
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (!file) {
        printf("Error: Failed to open file '%s'.\n", filename);
        return;
    }

    // User input for the new content of the file
    printf("Enter new content for file '%s' (type END to finish):\n", filename);
    char input[256];
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strncmp(input, "END", 3) == 0) break; // Finish input
        fprintf(file, "%s", input); // Write line to file
    }

    fclose(file);
    printf("File '%s' updated successfully.\n", filename);
}

// Read file contents
void read_file(const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (!file) {
        printf("Error: Failed to open file '%s'.\n", filename);
        return;
    }

    // Read and display file contents
    printf("Contents of file '%s':\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Display help
void help() {
    printf("\nAvailable commands:\n");
    printf("create name memory  - Create a process\n");
    printf("delete id           - Delete a process\n");
    printf("list                - Show the list of processes\n");
    printf("createfile filename - Create a file\n");
    printf("deletefile filename - Delete a file\n");
    printf("editfile filename   - Edit a file\n");
    printf("start filename      - Read file contents\n");
    printf("display text        - Display text\n");
    printf("help                - Show help\n");
    printf("exit                - Exit SimeOS\n");
}

// Main menu
void menu() {
    char command[256];
    while (1) {
        printf("\nSimeOS$> ");
        scanf(" %[^\n]", command);

        if (strncmp(command, "create ", 7) == 0) { // Create process
            char name[20];
            int memory;
            sscanf(command, "create %s %d", name, &memory);
            create_process(name, memory);
        } else if (strncmp(command, "delete ", 7) == 0) { // Delete process
            int id;
            sscanf(command, "delete %d", &id);
            delete_process(id);
        } else if (strcmp(command, "list") == 0) { // List processes
            list_processes();
        } else if (strncmp(command, "createfile ", 11) == 0) { // Create file
            char filename[100];
            sscanf(command, "createfile %s", filename);
            create_file(filename);
        } else if (strncmp(command, "deletefile ", 11) == 0) { // Delete file
            char filename[100];
            sscanf(command, "deletefile %s", filename);
            delete_file(filename);
        } else if (strncmp(command, "editfile ", 9) == 0) { // Edit file
            char filename[100];
            sscanf(command, "editfile %s", filename);
            getchar(); // Remove extra newline
            edit_file(filename);
        } else if (strncmp(command, "start ", 6) == 0) { // Read file
            char filename[100];
            sscanf(command, "start %s", filename);
            read_file(filename);
        } else if (strncmp(command, "display ", 8) == 0) { // Display text
            char text[200];
            sscanf(command, "display %[^\n]", text);
            display(text);
        } else if (strcmp(command, "help") == 0) { // Help
            help();
        } else if (strcmp(command, "exit") == 0) { // Exit
            display("Exiting SimeOS...");
            break;
        } else {
            display("Unknown command. Type help for assistance.");
        }
    }
}

// Entry point
int main() {
    display("Welcome to SimeOS!");
    menu();
    return 0;
}
