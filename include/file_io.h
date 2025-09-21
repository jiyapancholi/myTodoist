/**
 * @file file_io.h
 * @brief Header file for file I/O operations for todo list persistence
 * @author Jiya Pancholi
 * @date 2025-09-21
 * 
 * This file contains function declarations for saving and loading
 * todo list data to/from files for persistence.
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include "todo.h"

// Default filename for todo list storage
#define DEFAULT_FILENAME "data/todos.dat"
#define DATA_DIR "data"

/**
 * @brief Save todo list to a binary file
 * @param list Pointer to the todo list to save
 * @param filename Name of the file to save to (NULL for default)
 * @return 0 on success, -1 on failure
 */
int save_todos_to_file(const TodoList* list, const char* filename);

/**
 * @brief Load todo list from a binary file
 * @param list Pointer to the todo list to load into
 * @param filename Name of the file to load from (NULL for default)
 * @return 0 on success, -1 on failure
 */
int load_todos_from_file(TodoList* list, const char* filename);

/**
 * @brief Export todo list to a human-readable text file
 * @param list Pointer to the todo list to export
 * @param filename Name of the text file to export to
 * @return 0 on success, -1 on failure
 */
int export_todos_to_text(const TodoList* list, const char* filename);

/**
 * @brief Check if a file exists
 * @param filename Name of the file to check
 * @return 1 if file exists, 0 otherwise
 */
int file_exists(const char* filename);

/**
 * @brief Ensure data directory exists
 * @return 0 on success, -1 on failure
 */
int ensure_data_directory(void);

/**
 * @brief Create a backup of the current todo file
 * @param filename Name of the file to backup
 * @return 0 on success, -1 on failure
 */
int create_backup(const char* filename);

#endif // FILE_IO_H