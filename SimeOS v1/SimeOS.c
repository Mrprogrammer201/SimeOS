#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PROCESSES 100

typedef struct {
    char username[50];
    char password[50];
} User;

typedef struct {
    int id;
    char name[50];
} Process;

User currentUser;
Process processes[MAX_PROCESSES];
int processCount = 0;
int memorySize = 0;

void set_console_encoding() {
    SetConsoleCP(65001);        // Set input encoding to UTF-8
    SetConsoleOutputCP(65001);  // Set output encoding to UTF-8
}

void save_state() {
    FILE *file = fopen("simeos_state.txt", "w");
    if (file) {
        fprintf(file, "Username:%s\n", currentUser.username);
        fprintf(file, "Password:%s\n", currentUser.password);
        fprintf(file, "Memory:%d\n", memorySize);
        fprintf(file, "Processes:%d\n", processCount);
        for (int i = 0; i < processCount; i++) {
            fprintf(file, "Process:%d:%s\n", processes[i].id, processes[i].name);
        }
        fclose(file);
    }
}

void load_state() {
    FILE *file = fopen("simeos_state.txt", "r");
    if (file) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "Username:", 9) == 0) {
                sscanf(line + 9, "%s", currentUser.username);
            } else if (strncmp(line, "Password:", 9) == 0) {
                sscanf(line + 9, "%s", currentUser.password);
            } else if (strncmp(line, "Memory:", 7) == 0) {
                sscanf(line + 7, "%d", &memorySize);
            } else if (strncmp(line, "Processes:", 10) == 0) {
                sscanf(line + 10, "%d", &processCount);
            } else if (strncmp(line, "Process:", 8) == 0) {
                sscanf(line + 8, "%d:%49[^\n]", &processes[processCount].id, processes[processCount].name);
                processCount++;
            }
        }
        fclose(file);
    }
}

void setup_system() {
    printf("Enter total memory for OS: ");
    scanf("%d", &memorySize);
    getchar(); // Clear newline character
    printf("Enter PC name: ");
    fgets(currentUser.username, sizeof(currentUser.username), stdin);
    currentUser.username[strcspn(currentUser.username, "\n")] = '\0';
    printf("Enter password for PC: ");
    fgets(currentUser.password, sizeof(currentUser.password), stdin);
    currentUser.password[strcspn(currentUser.password, "\n")] = '\0';
    save_state();
}

void add_process() {
    if (processCount >= MAX_PROCESSES) {
        printf("Cannot add more processes. Maximum limit reached.\n");
        return;
    }
    Process newProcess;
    newProcess.id = processCount + 1;
    printf("Enter process name: ");
    fgets(newProcess.name, sizeof(newProcess.name), stdin);
    newProcess.name[strcspn(newProcess.name, "\n")] = '\0';
    processes[processCount++] = newProcess;
    save_state();
    printf("Process '%s' added successfully.\n", newProcess.name);
}

void delete_process() {
    int id;
    printf("Enter process ID to delete: ");
    scanf("%d", &id);
    getchar(); // Clear newline character
    for (int i = 0; i < processCount; i++) {
        if (processes[i].id == id) {
            for (int j = i; j < processCount - 1; j++) {
                processes[j] = processes[j + 1];
            }
            processCount--;
            save_state();
            printf("Process deleted successfully.\n");
            return;
        }
    }
    printf("Process not found.\n");
}

void list_processes() {
    printf("Processes:\n");
    for (int i = 0; i < processCount; i++) {
        printf("ID: %d, Name: %s\n", processes[i].id, processes[i].name);
    }
}

void allocate_memory() {
    int memory;
    printf("Enter memory to allocate: ");
    scanf("%d", &memory);
    getchar(); // Clear newline character
    if (memory <= memorySize) {
        memorySize -= memory;
        save_state();
        printf("Memory allocated successfully. Remaining memory: %d\n", memorySize);
    } else {
        printf("Not enough memory available.\n");
    }
}

void view_memory() {
    printf("Available memory: %d\n", memorySize);
}

void delete_profile() {
    char confirmation[10];
    printf("Are you sure you want to delete the user profile? Type 'yes' to confirm: ");
    fgets(confirmation, sizeof(confirmation), stdin);
    confirmation[strcspn(confirmation, "\n")] = '\0';
    if (strcmp(confirmation, "yes") == 0) {
        memset(&currentUser, 0, sizeof(currentUser));
        memset(processes, 0, sizeof(processes));
        processCount = 0;
        memorySize = 0;
        remove("simeos_state.txt");
        printf("Profile deleted successfully.\n");
    } else {
        printf("Profile deletion canceled.\n");
    }
}

void display_help() {
    printf("Available commands:\n");
    printf("1. create_process - Add a new process\n");
    printf("2. delete_process - Remove an existing process\n");
    printf("3. list_processes - View all processes\n");
    printf("4. allocate_memory - Allocate memory\n");
    printf("5. view_memory - View remaining memory\n");
    printf("6. delete_profile - Delete the user profile\n");
    printf("7. exit - Exit the OS\n");
}

int main() {
    set_console_encoding();
    load_state();

    char command[50];

    if (strlen(currentUser.username) == 0) {
        printf("System setup required.\n");
        setup_system();
    }

    while (1) {
        printf("%s@SimeOS> ", currentUser.username);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "create_process") == 0) {
            add_process();
        } else if (strcmp(command, "delete_process") == 0) {
            delete_process();
        } else if (strcmp(command, "list_processes") == 0) {
            list_processes();
        } else if (strcmp(command, "allocate_memory") == 0) {
            allocate_memory();
        } else if (strcmp(command, "view_memory") == 0) {
            view_memory();
        } else if (strcmp(command, "delete_profile") == 0) {
            delete_profile();
        } else if (strcmp(command, "help") == 0) {
            display_help();
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting SimeOS...\n");
            break;
        } else {
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }

    return 0;
}
