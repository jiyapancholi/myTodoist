/**
 * @file file_io.c
 * @brief Implementation of file I/O operations for todo list persistence
 * @author Jiya Pancholi
 * @date 2025-09-21
 * 
 * This file implements functions for saving and loading todo list data
 * to/from files for persistence across program sessions.
 */

#include "../include/file_io.h"

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

/**
 * @brief Ensure data directory exists
 * @return 0 on success, -1 on failure
 */
int ensure_data_directory(void) {
    // Check if data directory exists
    FILE* test = fopen("data/.", "r");
    if (test) {
        fclose(test);
        return 0; // Directory exists
    }
    
    // Try to create the directory
#ifdef _WIN32
    if (_mkdir(DATA_DIR) == 0) {
#else
    if (mkdir(DATA_DIR, 0755) == 0) {
#endif
        return 0; // Successfully created
    }
    
    // Directory might already exist, try to verify
    test = fopen("data/.", "r");
    if (test) {
        fclose(test);
        return 0;
    }
    
    return -1; // Failed to create directory
}

/**
 * @brief Save todo list to a binary file
 * @param list Pointer to the todo list to save
 * @param filename Name of the file to save to (NULL for default)
 * @return 0 on success, -1 on failure
 */
int save_todos_to_file(const TodoList* list, const char* filename) {
    if (!list) {
        fprintf(stderr, "Error: Invalid todo list\n");
        return -1;
    }
    
    // Ensure data directory exists
    if (ensure_data_directory() != 0) {
        fprintf(stderr, "Warning: Could not create data directory\n");
    }
    
    const char* file_to_use = filename ? filename : DEFAULT_FILENAME;
    
    // Create backup before saving
    if (file_exists(file_to_use)) {
        create_backup(file_to_use);
    }
    
    FILE* file = fopen(file_to_use, "wb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s' for writing\n", file_to_use);
        return -1;
    }
    
    // Write header information
    if (fwrite(&list->count, sizeof(int), 1, file) != 1 ||
        fwrite(&list->next_id, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to write header to file\n");
        fclose(file);
        return -1;
    }
    
    // Write todo items
    if (list->count > 0) {
        if (fwrite(list->todos, sizeof(Todo), list->count, file) != (size_t)list->count) {
            fprintf(stderr, "Error: Failed to write todos to file\n");
            fclose(file);
            return -1;
        }
    }
    
    fclose(file);
    printf("Successfully saved %d todos to '%s'\n", list->count, file_to_use);
    return 0;
}

/**
 * @brief Load todo list from a binary file
 * @param list Pointer to the todo list to load into
 * @param filename Name of the file to load from (NULL for default)
 * @return 0 on success, -1 on failure
 */
int load_todos_from_file(TodoList* list, const char* filename) {
    if (!list) {
        fprintf(stderr, "Error: Invalid todo list\n");
        return -1;
    }
    
    const char* file_to_use = filename ? filename : DEFAULT_FILENAME;
    
    if (!file_exists(file_to_use)) {
        printf("No existing todo file found. Starting with empty list.\n");
        return 0; // Not an error, just no existing data
    }
    
    FILE* file = fopen(file_to_use, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s' for reading\n", file_to_use);
        return -1;
    }
    
    // Read header information
    int saved_count, saved_next_id;
    if (fread(&saved_count, sizeof(int), 1, file) != 1 ||
        fread(&saved_next_id, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read header from file\n");
        fclose(file);
        return -1;
    }
    
    // Validate data
    if (saved_count < 0 || saved_count > MAX_TODOS) {
        fprintf(stderr, "Error: Invalid todo count in file (%d)\n", saved_count);
        fclose(file);
        return -1;
    }
    
    // Read todo items
    if (saved_count > 0) {
        if (fread(list->todos, sizeof(Todo), saved_count, file) != (size_t)saved_count) {
            fprintf(stderr, "Error: Failed to read todos from file\n");
            fclose(file);
            return -1;
        }
    }
    
    list->count = saved_count;
    list->next_id = saved_next_id;
    
    fclose(file);
    printf("Successfully loaded %d todos from '%s'\n", list->count, file_to_use);
    return 0;
}

/**
 * @brief Export todo list to a human-readable text file
 * @param list Pointer to the todo list to export
 * @param filename Name of the text file to export to
 * @return 0 on success, -1 on failure
 */
int export_todos_to_text(const TodoList* list, const char* filename) {
    if (!list || !filename) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return -1;
    }
    
    // Ensure data directory exists if filename starts with "data/"
    if (strncmp(filename, "data/", 5) == 0) {
        if (ensure_data_directory() != 0) {
            fprintf(stderr, "Warning: Could not create data directory\n");
        }
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s' for writing\n", filename);
        return -1;
    }
    
    fprintf(file, "=== TODO LIST EXPORT ===\n");
    fprintf(file, "Export Date: %s\n", ctime(&(time_t){time(NULL)}));
    fprintf(file, "Total Todos: %d\n\n", list->count);
    
    if (list->count == 0) {
        fprintf(file, "No todos found.\n");
    } else {
        for (int i = 0; i < list->count; i++) {
            const Todo* todo = &list->todos[i];
            char created_str[26], updated_str[26];
            
            // Format timestamps
            strcpy(created_str, ctime(&todo->created_at));
            strcpy(updated_str, ctime(&todo->updated_at));
            
            // Remove newlines from ctime strings
            created_str[24] = '\0';
            updated_str[24] = '\0';
            
            fprintf(file, "--- Todo #%d ---\n", todo->id);
            fprintf(file, "Title: %s\n", todo->title);
            fprintf(file, "Description: %s\n", todo->description[0] ? todo->description : "(No description)");
            fprintf(file, "Priority: %s\n", get_priority_string(todo->priority));
            fprintf(file, "Status: %s\n", get_status_string(todo->status));
            fprintf(file, "Created: %s\n", created_str);
            fprintf(file, "Updated: %s\n", updated_str);
            fprintf(file, "\n");
        }
    }
    
    fclose(file);
    printf("Successfully exported todos to '%s'\n", filename);
    return 0;
}

/**
 * @brief Check if a file exists
 * @param filename Name of the file to check
 * @return 1 if file exists, 0 otherwise
 */
int file_exists(const char* filename) {
    if (!filename) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * @brief Create a backup of the current todo file
 * @param filename Name of the file to backup
 * @return 0 on success, -1 on failure
 */
int create_backup(const char* filename) {
    if (!filename || !file_exists(filename)) {
        return -1;
    }
    
    // Create backup filename
    char backup_filename[256];
    snprintf(backup_filename, sizeof(backup_filename), "%s.backup", filename);
    
    FILE* source = fopen(filename, "rb");
    if (!source) {
        return -1;
    }
    
    FILE* dest = fopen(backup_filename, "wb");
    if (!dest) {
        fclose(source);
        return -1;
    }
    
    // Copy file contents
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) != bytes) {
            fclose(source);
            fclose(dest);
            return -1;
        }
    }
    
    fclose(source);
    fclose(dest);
    
    return 0;
}