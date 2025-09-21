/**
 * @file todo.h
 * @brief Header file for Todo List data structures and operations
 * @author Jiya Pancholi
 * @date 2025-09-21
 * 
 * This file contains the data structures and function declarations
 * for managing a todo list with CRUD operations.
 */

#ifndef TODO_H
#define TODO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Maximum string lengths
#define MAX_TITLE_LENGTH 100
#define MAX_DESC_LENGTH 500
#define MAX_TODOS 1000

/**
 * @brief Enumeration for todo priority levels
 */
typedef enum {
    PRIORITY_LOW = 1,
    PRIORITY_MEDIUM = 2,
    PRIORITY_HIGH = 3
} Priority;

/**
 * @brief Enumeration for todo status
 */
typedef enum {
    STATUS_PENDING = 0,
    STATUS_COMPLETED = 1
} Status;

/**
 * @brief Structure representing a single todo item
 */
typedef struct {
    int id;                                  /**< Unique identifier */
    char title[MAX_TITLE_LENGTH];           /**< Todo title */
    char description[MAX_DESC_LENGTH];      /**< Todo description */
    Priority priority;                      /**< Priority level */
    Status status;                         /**< Completion status */
    time_t created_at;                     /**< Creation timestamp */
    time_t updated_at;                     /**< Last update timestamp */
} Todo;

/**
 * @brief Structure for managing the todo list
 */
typedef struct {
    Todo* todos;                           /**< Dynamic array of todos */
    int count;                            /**< Current number of todos */
    int capacity;                         /**< Maximum capacity */
    int next_id;                         /**< Next available ID */
} TodoList;

// Function declarations for CRUD operations

/**
 * @brief Initialize a new todo list
 * @return Pointer to initialized TodoList, NULL on failure
 */
TodoList* todo_list_create(void);

/**
 * @brief Free memory allocated for todo list
 * @param list Pointer to the todo list to destroy
 */
void todo_list_destroy(TodoList* list);

/**
 * @brief Create a new todo item
 * @param list Pointer to the todo list
 * @param title Title of the todo
 * @param description Description of the todo
 * @param priority Priority level
 * @return ID of created todo, -1 on failure
 */
int todo_create(TodoList* list, const char* title, const char* description, Priority priority);

/**
 * @brief Read/display all todos
 * @param list Pointer to the todo list
 */
void todo_read_all(const TodoList* list);

/**
 * @brief Read/display a specific todo by ID
 * @param list Pointer to the todo list
 * @param id ID of the todo to display
 * @return 0 on success, -1 if not found
 */
int todo_read_by_id(const TodoList* list, int id);

/**
 * @brief Update a todo item
 * @param list Pointer to the todo list
 * @param id ID of the todo to update
 * @param title New title (NULL to keep existing)
 * @param description New description (NULL to keep existing)
 * @param priority New priority (-1 to keep existing)
 * @return 0 on success, -1 if not found
 */
int todo_update(TodoList* list, int id, const char* title, const char* description, int priority);

/**
 * @brief Delete a todo item
 * @param list Pointer to the todo list
 * @param id ID of the todo to delete
 * @return 0 on success, -1 if not found
 */
int todo_delete(TodoList* list, int id);

/**
 * @brief Mark a todo as completed
 * @param list Pointer to the todo list
 * @param id ID of the todo to complete
 * @return 0 on success, -1 if not found
 */
int todo_complete(TodoList* list, int id);

/**
 * @brief Mark a todo as pending
 * @param list Pointer to the todo list
 * @param id ID of the todo to mark pending
 * @return 0 on success, -1 if not found
 */
int todo_mark_pending(TodoList* list, int id);

/**
 * @brief Find a todo by ID
 * @param list Pointer to the todo list
 * @param id ID to search for
 * @return Pointer to todo if found, NULL otherwise
 */
Todo* todo_find_by_id(const TodoList* list, int id);

/**
 * @brief Get priority string representation
 * @param priority Priority enum value
 * @return String representation of priority
 */
const char* get_priority_string(Priority priority);

/**
 * @brief Get status string representation
 * @param status Status enum value
 * @return String representation of status
 */
const char* get_status_string(Status status);

#endif // TODO_H