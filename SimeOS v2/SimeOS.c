#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Максимальное количество процессов и общий размер памяти
#define MAX_PROCESSES 10
#define MEMORY_SIZE 1024

// Структура для хранения информации о процессе
typedef struct {
    int id; // ID процесса
    char name[20]; // Имя процесса
    int memory_used; // Память, используемая процессом
} Process;

Process processes[MAX_PROCESSES]; // Массив процессов
int process_count = 0; // Текущее количество процессов
int memory_used = 0; // Общий использованный объем памяти

// Функция для отображения текста
void display(const char *text) {
    printf("%s\n", text);
}

// Создание процесса
void create_process(char *name, int memory) {
    if (process_count >= MAX_PROCESSES) { // Проверка, если достигнут лимит процессов
        display("Ошибка: Достигнут предел количества процессов.");
        return;
    }
    if (memory_used + memory > MEMORY_SIZE) { // Проверка на достаточность памяти
        display("Ошибка: Недостаточно памяти.");
        return;
    }
    // Создаем новый процесс
    processes[process_count].id = process_count + 1;
    strcpy(processes[process_count].name, name);
    processes[process_count].memory_used = memory;
    process_count++;
    memory_used += memory;
    display("Процесс создан.");
}

// Удаление процесса
void delete_process(int id) {
    int found = 0; // Флаг для проверки, существует ли процесс
    for (int i = 0; i < process_count; i++) {
        if (processes[i].id == id) {
            // Освобождаем память и сдвигаем оставшиеся процессы
            memory_used -= processes[i].memory_used;
            for (int j = i; j < process_count - 1; j++) {
                processes[j] = processes[j + 1];
            }
            process_count--;
            found = 1;
            display("Процесс удален.");
            break;
        }
    }
    if (!found) display("Ошибка: Процесс не найден.");
}

// Отображение списка процессов
void list_processes() {
    printf("Список процессов:\n");
    for (int i = 0; i < process_count; i++) {
        printf("ID: %d, Имя: %s, Используемая память: %d\n",
               processes[i].id, processes[i].name, processes[i].memory_used);
    }
    printf("Общий использованный объем памяти: %d/%d\n", memory_used, MEMORY_SIZE);
}

// Создание файла
void create_file(const char *filename) {
    FILE *file = fopen(filename, "w"); // Открытие файла для записи
    if (file) {
        fclose(file);
        printf("Файл '%s' успешно создан.\n", filename);
    } else {
        printf("Ошибка: Не удалось создать файл '%s'.\n", filename);
    }
}

// Удаление файла
void delete_file(const char *filename) {
    if (unlink(filename) == 0) { // Удаление файла
        printf("Файл '%s' успешно удален.\n", filename);
    } else {
        printf("Ошибка: Не удалось удалить файл '%s'.\n", filename);
    }
}

// Редактирование файла
void edit_file(const char *filename) {
    FILE *file = fopen(filename, "w"); // Открытие файла для записи
    if (!file) {
        printf("Ошибка: Не удалось открыть файл '%s'.\n", filename);
        return;
    }

    // Ввод нового содержимого для файла
    printf("Введите новый контент для файла '%s' (наберите END для завершения):\n", filename);
    char input[256];
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strncmp(input, "END", 3) == 0) break; // Завершаем ввод
        fprintf(file, "%s", input); // Записываем строку в файл
    }

    fclose(file);
    printf("Файл '%s' успешно обновлен.\n", filename);
}

// Чтение содержимого файла
void read_file(const char *filename) {
    FILE *file = fopen(filename, "r"); // Открытие файла для чтения
    if (!file) {
        printf("Ошибка: Не удалось открыть файл '%s'.\n", filename);
        return;
    }

    // Чтение и вывод содержимого файла
    printf("Содержимое файла '%s':\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Отображение справки
void help() {
    printf("\nДоступные команды:\n");
    printf("create name memory  - Создать процесс\n");
    printf("delete id           - Удалить процесс\n");
    printf("list                - Показать список процессов\n");
    printf("createfile filename - Создать файл\n");
    printf("deletefile filename - Удалить файл\n");
    printf("editfile filename   - Редактировать файл\n");
    printf("start filename      - Прочитать содержимое файла\n");
    printf("display text        - Показать текст\n");
    printf("help                - Показать справку\n");
    printf("exit                - Выход из SimeOS\n");
}

// Главное меню
void menu() {
    char command[256];
    while (1) {
        printf("\nSimeOS$> ");
        scanf(" %[^\n]", command);

        if (strncmp(command, "create ", 7) == 0) { // Создание процесса
            char name[20];
            int memory;
            sscanf(command, "create %s %d", name, &memory);
            create_process(name, memory);
        } else if (strncmp(command, "delete ", 7) == 0) { // Удаление процесса
            int id;
            sscanf(command, "delete %d", &id);
            delete_process(id);
        } else if (strcmp(command, "list") == 0) { // Список процессов
            list_processes();
        } else if (strncmp(command, "createfile ", 11) == 0) { // Создание файла
            char filename[100];
            sscanf(command, "createfile %s", filename);
            create_file(filename);
        } else if (strncmp(command, "deletefile ", 11) == 0) { // Удаление файла
            char filename[100];
            sscanf(command, "deletefile %s", filename);
            delete_file(filename);
        } else if (strncmp(command, "editfile ", 9) == 0) { // Редактирование файла
            char filename[100];
            sscanf(command, "editfile %s", filename);
            getchar(); // Убираем лишний символ новой строки
            edit_file(filename);
        } else if (strncmp(command, "start ", 6) == 0) { // Чтение файла
            char filename[100];
            sscanf(command, "start %s", filename);
            read_file(filename);
        } else if (strncmp(command, "display ", 8) == 0) { // Показать текст
            char text[200];
            sscanf(command, "display %[^\n]", text);
            display(text);
        } else if (strcmp(command, "help") == 0) { // Справка
            help();
        } else if (strcmp(command, "exit") == 0) { // Выход
            display("Выход из SimeOS...");
            break;
        } else {
            display("Неизвестная команда. Введите help для получения помощи.");
        }
    }
}

// Точка входа
int main() {
    display("Добро пожаловать в SimeOS!");
    menu();
    return 0;
}
