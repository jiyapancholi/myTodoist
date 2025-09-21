/**
 * @file main.c
 * @brief Main program for the Todo List application
 * @author Jiya Pancholi
 * @date 2025-09-21
 * 
 * This file contains the main function and user interface for the
 * Todo List application with CRUD operations.
 */

#include "../include/todo.h"
#include "../include/file_io.h"

// Function prototypes for menu functions
void show_menu(void);
void handle_create_todo(TodoList* list);
void handle_update_todo(TodoList* list);
void handle_delete_todo(TodoList* list);
void handle_complete_todo(TodoList* list);
void handle_view_todo(TodoList* list);
void handle_export_todos(TodoList* list);
void clear_input_buffer(void);
int get_integer_input(const char* prompt);
Priority get_priority_input(void);
void get_string_input(const char* prompt, char* buffer, int max_length);

/**
 * @brief Main function - entry point of the program
 * @return 0 on successful execution, -1 on error
 */
int main(void) {
    printf("=== Todo List Manager ===\n");
    printf("Welcome to your personal todo list!\n\n");
    
    // Initialize todo list
    TodoList* todo_list = todo_list_create();
    if (!todo_list) {
        fprintf(stderr, "Failed to initialize todo list\n");
        return -1;
    }
    
    // Load existing todos from file
    load_todos_from_file(todo_list, NULL);
    
    int choice;
    int running = 1;
    
    while (running) {
        show_menu();
        choice = get_integer_input("Enter your choice");
        
        // Handle EOF or input error
        if (choice == -1) {
            printf("\nInput stream ended. Exiting...\n");
            save_todos_to_file(todo_list, NULL);
            running = 0;
            break;
        }
        
        switch (choice) {
            case 1:
                handle_create_todo(todo_list);
                break;
            case 2:
                todo_read_all(todo_list);
                break;
            case 3:
                handle_view_todo(todo_list);
                break;
            case 4:
                handle_update_todo(todo_list);
                break;
            case 5:
                handle_delete_todo(todo_list);
                break;
            case 6:
                handle_complete_todo(todo_list);
                break;
            case 7:
                save_todos_to_file(todo_list, NULL);
                break;
            case 8:
                handle_export_todos(todo_list);
                break;
            case 9:
                printf("Saving todos before exit...\n");
                save_todos_to_file(todo_list, NULL);
                running = 0;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        
        if (running) {
            printf("\nPress Enter to continue...");
            // Handle EOF in getchar as well
            int c = getchar();
            if (c == EOF) {
                printf("\nInput stream ended. Exiting...\n");
                save_todos_to_file(todo_list, NULL);
                running = 0;
            }
        }
    }
    
    // Cleanup
    todo_list_destroy(todo_list);
    printf("\nThank you for using Todo List Manager!\n");
    return 0;
}

/**
 * @brief Display the main menu options
 */
void show_menu(void) {
    printf("\n=== MAIN MENU ===\n");
    printf("1. Create new todo\n");
    printf("2. View all todos\n");
    printf("3. View specific todo\n");
    printf("4. Update todo\n");
    printf("5. Delete todo\n");
    printf("6. Mark todo as completed/pending\n");
    printf("7. Save todos to file\n");
    printf("8. Export todos to text file\n");
    printf("9. Exit\n");
    printf("================\n");
}

/**
 * @brief Handle creating a new todo
 * @param list Pointer to the todo list
 */
void handle_create_todo(TodoList* list) {
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESC_LENGTH];
    
    printf("\n=== CREATE NEW TODO ===\n");
    
    get_string_input("Enter todo title", title, MAX_TITLE_LENGTH);
    
    printf("Enter todo description (optional, press Enter to skip): ");
    if (fgets(description, MAX_DESC_LENGTH, stdin)) {
        // Remove newline character
        char* newline = strchr(description, '\n');
        if (newline) *newline = '\0';
    }
    
    Priority priority = get_priority_input();
    
    int id = todo_create(list, title, description[0] ? description : NULL, priority);
    if (id != -1) {
        printf("\nTodo created successfully!\n");
    }
}

/**
 * @brief Handle updating a todo
 * @param list Pointer to the todo list
 */
void handle_update_todo(TodoList* list) {
    if (list->count == 0) {
        printf("No todos available to update.\n");
        return;
    }
    
    printf("\n=== UPDATE TODO ===\n");
    todo_read_all(list);
    
    int id = get_integer_input("\nEnter todo ID to update");
    
    // Check if todo exists
    if (!todo_find_by_id(list, id)) {
        printf("Todo with ID %d not found.\n", id);
        return;
    }
    
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESC_LENGTH];
    char input[10];
    
    printf("Update title? (y/n): ");
    fgets(input, sizeof(input), stdin);
    char* new_title = NULL;
    if (input[0] == 'y' || input[0] == 'Y') {
        get_string_input("Enter new title", title, MAX_TITLE_LENGTH);
        new_title = title;
    }
    
    printf("Update description? (y/n): ");
    fgets(input, sizeof(input), stdin);
    char* new_description = NULL;
    if (input[0] == 'y' || input[0] == 'Y') {
        printf("Enter new description: ");
        if (fgets(description, MAX_DESC_LENGTH, stdin)) {
            char* newline = strchr(description, '\n');
            if (newline) *newline = '\0';
            new_description = description;
        }
    }
    
    printf("Update priority? (y/n): ");
    fgets(input, sizeof(input), stdin);
    int new_priority = -1;
    if (input[0] == 'y' || input[0] == 'Y') {
        new_priority = get_priority_input();
    }
    
    todo_update(list, id, new_title, new_description, new_priority);
}

/**
 * @brief Handle deleting a todo
 * @param list Pointer to the todo list
 */
void handle_delete_todo(TodoList* list) {
    if (list->count == 0) {
        printf("No todos available to delete.\n");
        return;
    }
    
    printf("\n=== DELETE TODO ===\n");
    todo_read_all(list);
    
    int id = get_integer_input("\nEnter todo ID to delete");
    
    char confirmation[10];
    printf("Are you sure you want to delete todo with ID %d? (y/n): ", id);
    fgets(confirmation, sizeof(confirmation), stdin);
    
    if (confirmation[0] == 'y' || confirmation[0] == 'Y') {
        todo_delete(list, id);
    } else {
        printf("Delete operation cancelled.\n");
    }
}

/**
 * @brief Handle marking todo as completed or pending
 * @param list Pointer to the todo list
 */
void handle_complete_todo(TodoList* list) {
    if (list->count == 0) {
        printf("No todos available.\n");
        return;
    }
    
    printf("\n=== CHANGE TODO STATUS ===\n");
    todo_read_all(list);
    
    int id = get_integer_input("\nEnter todo ID");
    
    printf("1. Mark as completed\n");
    printf("2. Mark as pending\n");
    int choice = get_integer_input("Enter your choice");
    
    switch (choice) {
        case 1:
            todo_complete(list, id);
            break;
        case 2:
            todo_mark_pending(list, id);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
}

/**
 * @brief Handle viewing a specific todo
 * @param list Pointer to the todo list
 */
void handle_view_todo(TodoList* list) {
    if (list->count == 0) {
        printf("No todos available to view.\n");
        return;
    }
    
    int id = get_integer_input("Enter todo ID to view");
    todo_read_by_id(list, id);
}

/**
 * @brief Handle exporting todos to text file
 * @param list Pointer to the todo list
 */
void handle_export_todos(TodoList* list) {
    char filename[256];
    
    get_string_input("Enter filename for export (e.g., data/todos.txt)", filename, sizeof(filename));
    export_todos_to_text(list, filename);
}

/**
 * @brief Clear input buffer to handle invalid input
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Get integer input from user with validation
 * @param prompt Prompt message to display
 * @return Integer value entered by user
 */
int get_integer_input(const char* prompt) {
    int value;
    char buffer[100];
    int attempts = 0;
    const int max_attempts = 10;
    
    while (attempts < max_attempts) {
        printf("%s: ", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Check if we got valid integer input
            char *endptr;
            value = strtol(buffer, &endptr, 10);
            
            // Valid if entire string was consumed and no overflow occurred
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return value;
            }
        } else {
            // EOF or error - return -1 to indicate failure
            printf("\nEnd of input reached or error occurred.\n");
            return -1;
        }
        
        printf("Invalid input. Please enter a valid number.\n");
        attempts++;
    }
    
    printf("Too many invalid attempts. Using default value 0.\n");
    return 0;
}

/**
 * @brief Get priority input from user
 * @return Priority value selected by user
 */
Priority get_priority_input(void) {
    printf("\nPriority levels:\n");
    printf("1. Low\n");
    printf("2. Medium\n");
    printf("3. High\n");
    
    int choice;
    int attempts = 0;
    const int max_attempts = 5;
    
    while (attempts < max_attempts) {
        choice = get_integer_input("Enter priority");
        
        // Handle EOF or error case
        if (choice == -1) {
            printf("Using default priority: Medium\n");
            return PRIORITY_MEDIUM;
        }
        
        if (choice >= 1 && choice <= 3) {
            return (Priority)choice;
        }
        
        printf("Invalid priority. Please enter 1, 2, or 3.\n");
        attempts++;
    }
    
    printf("Too many invalid attempts. Using default priority: Medium\n");
    return PRIORITY_MEDIUM;
}

/**
 * @brief Get string input from user with validation
 * @param prompt Prompt message to display
 * @param buffer Buffer to store input
 * @param max_length Maximum length of input
 */
void get_string_input(const char* prompt, char* buffer, int max_length) {
    int attempts = 0;
    const int max_attempts = 5;
    
    while (attempts < max_attempts) {
        printf("%s: ", prompt);
        if (fgets(buffer, max_length, stdin) != NULL) {
            // Remove newline character
            char* newline = strchr(buffer, '\n');
            if (newline) *newline = '\0';
            
            // Check if input is empty
            if (strlen(buffer) > 0) {
                return;
            }
        } else {
            // EOF reached
            printf("\nEnd of input reached. Using default value.\n");
            strcpy(buffer, "Default");
            return;
        }
        
        printf("Input cannot be empty. Please try again.\n");
        attempts++;
    }
    
    printf("Too many invalid attempts. Using default value.\n");
    strcpy(buffer, "Default");
}