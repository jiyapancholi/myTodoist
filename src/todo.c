/**
 * @file todo.c
 * @brief Implementation of Todo List data structures and operations
 * @author Jiya Pancholi
 * @date 2025-09-21
 * 
 * This file implements the CRUD operations for managing a todo list.
 */

#include "../include/todo.h"

/**
 * @brief Initialize a new todo list
 * @return Pointer to initialized TodoList, NULL on failure
 */
TodoList* todo_list_create(void) {
    TodoList* list = (TodoList*)malloc(sizeof(TodoList));
    if (!list) {
        fprintf(stderr, "Error: Memory allocation failed for TodoList\n");
        return NULL;
    }
    
    list->todos = (Todo*)malloc(sizeof(Todo) * MAX_TODOS);
    if (!list->todos) {
        fprintf(stderr, "Error: Memory allocation failed for todos array\n");
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = MAX_TODOS;
    list->next_id = 1;
    
    return list;
}

/**
 * @brief Free memory allocated for todo list
 * @param list Pointer to the todo list to destroy
 */
void todo_list_destroy(TodoList* list) {
    if (list) {
        if (list->todos) {
            free(list->todos);
        }
        free(list);
    }
}

/**
 * @brief Create a new todo item
 * @param list Pointer to the todo list
 * @param title Title of the todo
 * @param description Description of the todo
 * @param priority Priority level
 * @return ID of created todo, -1 on failure
 */
int todo_create(TodoList* list, const char* title, const char* description, Priority priority) {
    if (!list || !title || list->count >= list->capacity) {
        fprintf(stderr, "Error: Invalid parameters or list is full\n");
        return -1;
    }
    
    // Validate input lengths
    if (strlen(title) >= MAX_TITLE_LENGTH) {
        fprintf(stderr, "Error: Title too long (max %d characters)\n", MAX_TITLE_LENGTH - 1);
        return -1;
    }
    
    if (description && strlen(description) >= MAX_DESC_LENGTH) {
        fprintf(stderr, "Error: Description too long (max %d characters)\n", MAX_DESC_LENGTH - 1);
        return -1;
    }
    
    // Get current time
    time_t now = time(NULL);
    
    // Create new todo
    Todo* new_todo = &list->todos[list->count];
    new_todo->id = list->next_id++;
    strncpy(new_todo->title, title, MAX_TITLE_LENGTH - 1);
    new_todo->title[MAX_TITLE_LENGTH - 1] = '\0';
    
    if (description) {
        strncpy(new_todo->description, description, MAX_DESC_LENGTH - 1);
        new_todo->description[MAX_DESC_LENGTH - 1] = '\0';
    } else {
        new_todo->description[0] = '\0';
    }
    
    new_todo->priority = priority;
    new_todo->status = STATUS_PENDING;
    new_todo->created_at = now;
    new_todo->updated_at = now;
    
    list->count++;
    
    printf("Todo created successfully with ID: %d\n", new_todo->id);
    return new_todo->id;
}

/**
 * @brief Read/display all todos
 * @param list Pointer to the todo list
 */
void todo_read_all(const TodoList* list) {
    if (!list) {
        fprintf(stderr, "Error: Invalid todo list\n");
        return;
    }
    
    if (list->count == 0) {
        printf("No todos found.\n");
        return;
    }
    
    printf("\n=== TODO LIST ===\n");
    printf("%-4s | %-20s | %-10s | %-8s | %-19s | %-19s\n", 
           "ID", "Title", "Priority", "Status", "Created", "Updated");
    printf("-----|----------------------|------------|----------|---------------------|---------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        const Todo* todo = &list->todos[i];
        char created_str[20], updated_str[20];
        
        // Format timestamps
        strftime(created_str, sizeof(created_str), "%Y-%m-%d %H:%M", localtime(&todo->created_at));
        strftime(updated_str, sizeof(updated_str), "%Y-%m-%d %H:%M", localtime(&todo->updated_at));
        
        printf("%-4d | %-20.20s | %-10s | %-8s | %-19s | %-19s\n",
               todo->id,
               todo->title,
               get_priority_string(todo->priority),
               get_status_string(todo->status),
               created_str,
               updated_str);
    }
    printf("\nTotal todos: %d\n", list->count);
}

/**
 * @brief Read/display a specific todo by ID
 * @param list Pointer to the todo list
 * @param id ID of the todo to display
 * @return 0 on success, -1 if not found
 */
int todo_read_by_id(const TodoList* list, int id) {
    const Todo* todo = todo_find_by_id(list, id);
    if (!todo) {
        printf("Todo with ID %d not found.\n", id);
        return -1;
    }
    
    char created_str[20], updated_str[20];
    strftime(created_str, sizeof(created_str), "%Y-%m-%d %H:%M:%S", localtime(&todo->created_at));
    strftime(updated_str, sizeof(updated_str), "%Y-%m-%d %H:%M:%S", localtime(&todo->updated_at));
    
    printf("\n=== TODO DETAILS ===\n");
    printf("ID: %d\n", todo->id);
    printf("Title: %s\n", todo->title);
    printf("Description: %s\n", todo->description[0] ? todo->description : "(No description)");
    printf("Priority: %s\n", get_priority_string(todo->priority));
    printf("Status: %s\n", get_status_string(todo->status));
    printf("Created: %s\n", created_str);
    printf("Updated: %s\n", updated_str);
    printf("===================\n");
    
    return 0;
}

/**
 * @brief Update a todo item
 * @param list Pointer to the todo list
 * @param id ID of the todo to update
 * @param title New title (NULL to keep existing)
 * @param description New description (NULL to keep existing)
 * @param priority New priority (-1 to keep existing)
 * @return 0 on success, -1 if not found
 */
int todo_update(TodoList* list, int id, const char* title, const char* description, int priority) {
    Todo* todo = todo_find_by_id(list, id);
    if (!todo) {
        printf("Todo with ID %d not found.\n", id);
        return -1;
    }
    
    // Update title if provided
    if (title) {
        if (strlen(title) >= MAX_TITLE_LENGTH) {
            fprintf(stderr, "Error: Title too long (max %d characters)\n", MAX_TITLE_LENGTH - 1);
            return -1;
        }
        strncpy(todo->title, title, MAX_TITLE_LENGTH - 1);
        todo->title[MAX_TITLE_LENGTH - 1] = '\0';
    }
    
    // Update description if provided
    if (description) {
        if (strlen(description) >= MAX_DESC_LENGTH) {
            fprintf(stderr, "Error: Description too long (max %d characters)\n", MAX_DESC_LENGTH - 1);
            return -1;
        }
        strncpy(todo->description, description, MAX_DESC_LENGTH - 1);
        todo->description[MAX_DESC_LENGTH - 1] = '\0';
    }
    
    // Update priority if provided (valid range)
    if (priority >= PRIORITY_LOW && priority <= PRIORITY_HIGH) {
        todo->priority = (Priority)priority;
    }
    
    // Update timestamp
    todo->updated_at = time(NULL);
    
    printf("Todo with ID %d updated successfully.\n", id);
    return 0;
}

/**
 * @brief Delete a todo item
 * @param list Pointer to the todo list
 * @param id ID of the todo to delete
 * @return 0 on success, -1 if not found
 */
int todo_delete(TodoList* list, int id) {
    if (!list) {
        return -1;
    }
    
    // Find the todo index
    int index = -1;
    for (int i = 0; i < list->count; i++) {
        if (list->todos[i].id == id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Todo with ID %d not found.\n", id);
        return -1;
    }
    
    // Shift remaining todos to fill the gap
    for (int i = index; i < list->count - 1; i++) {
        list->todos[i] = list->todos[i + 1];
    }
    
    list->count--;
    printf("Todo with ID %d deleted successfully.\n", id);
    return 0;
}

/**
 * @brief Mark a todo as completed
 * @param list Pointer to the todo list
 * @param id ID of the todo to complete
 * @return 0 on success, -1 if not found
 */
int todo_complete(TodoList* list, int id) {
    Todo* todo = todo_find_by_id(list, id);
    if (!todo) {
        printf("Todo with ID %d not found.\n", id);
        return -1;
    }
    
    if (todo->status == STATUS_COMPLETED) {
        printf("Todo with ID %d is already completed.\n", id);
        return 0;
    }
    
    todo->status = STATUS_COMPLETED;
    todo->updated_at = time(NULL);
    
    printf("Todo with ID %d marked as completed.\n", id);
    return 0;
}

/**
 * @brief Mark a todo as pending
 * @param list Pointer to the todo list
 * @param id ID of the todo to mark pending
 * @return 0 on success, -1 if not found
 */
int todo_mark_pending(TodoList* list, int id) {
    Todo* todo = todo_find_by_id(list, id);
    if (!todo) {
        printf("Todo with ID %d not found.\n", id);
        return -1;
    }
    
    if (todo->status == STATUS_PENDING) {
        printf("Todo with ID %d is already pending.\n", id);
        return 0;
    }
    
    todo->status = STATUS_PENDING;
    todo->updated_at = time(NULL);
    
    printf("Todo with ID %d marked as pending.\n", id);
    return 0;
}

/**
 * @brief Find a todo by ID
 * @param list Pointer to the todo list
 * @param id ID to search for
 * @return Pointer to todo if found, NULL otherwise
 */
Todo* todo_find_by_id(const TodoList* list, int id) {
    if (!list) {
        return NULL;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->todos[i].id == id) {
            return &list->todos[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Get priority string representation
 * @param priority Priority enum value
 * @return String representation of priority
 */
const char* get_priority_string(Priority priority) {
    switch (priority) {
        case PRIORITY_LOW:    return "Low";
        case PRIORITY_MEDIUM: return "Medium";
        case PRIORITY_HIGH:   return "High";
        default:              return "Unknown";
    }
}

/**
 * @brief Get status string representation
 * @param status Status enum value
 * @return String representation of status
 */
const char* get_status_string(Status status) {
    switch (status) {
        case STATUS_PENDING:   return "Pending";
        case STATUS_COMPLETED: return "Completed";
        default:               return "Unknown";
    }
}