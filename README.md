# Todo List Manager

A comprehensive, modular C-based todo list application with full CRUD (Create, Read, Update, Delete) operations and persistent storage.

## Features

- ✅ **Full CRUD Operations**: Create, read, update, and delete todos
- 💾 **Persistent Storage**: Automatic saving/loading with binary file format
- 📊 **Priority Levels**: Low, Medium, and High priority classification
- ⏰ **Timestamps**: Automatic creation and update time tracking
- 📤 **Export Functionality**: Export todos to human-readable text files
- 🔒 **Data Safety**: Automatic backup creation before saving
- 🎯 **Status Tracking**: Mark todos as completed or pending
- 📋 **Detailed View**: View individual todo details or all todos at once

## Project Structure

```
cpplang/
├── src/            # Source code files
│   ├── main.c      # User interface and main program
│   ├── todo.c      # Core CRUD operations implementation
│   └── file_io.c   # Persistence and export operations
├── include/        # Header files
│   ├── todo.h      # Data structures and function declarations
│   └── file_io.h   # File I/O function declarations
├── build/          # Build artifacts and object files
├── data/           # Runtime data files (todos.dat, exports)
├── Makefile        # Unix/Linux build system
├── build.bat       # Windows build script
└── README.md       # This documentation
```

## Data Structure

### Todo Item
Each todo contains:
- **ID**: Unique identifier (auto-generated)
- **Title**: Brief description (max 100 characters)
- **Description**: Detailed description (max 500 characters, optional)
- **Priority**: Low (1), Medium (2), or High (3)
- **Status**: Pending (0) or Completed (1)
- **Created At**: Creation timestamp
- **Updated At**: Last modification timestamp

### Todo List
The todo list manages:
- Dynamic array of todos (max 1000 items)
- Current count of todos
- Next available ID for new todos

## Compilation

### Using Makefile (Recommended)
```bash
make                # Compile the program
make clean          # Remove object files
make rebuild        # Clean and recompile
### Using Makefile (Unix/Linux/macOS)
```bash
make                # Build the project
make debug          # Build with debug symbols
make release        # Build optimized version
make clean          # Clean build artifacts
make run            # Compile and run
```

### Using Build Script (Windows)
```cmd
build.bat           # Build the project on Windows
```

### Manual Compilation
```bash
# Unix/Linux/macOS
gcc -Wall -Wextra -std=c99 -Iinclude -o todo_manager src/main.c src/todo.c src/file_io.c

# Windows
gcc -Wall -Wextra -std=c99 -Iinclude -o todo_manager.exe src\main.c src\todo.c src\file_io.c
```

## Usage

### Running the Program
```bash
./todo_manager      # On Unix/Linux/macOS
todo_manager.exe    # On Windows
```

### Menu Options
1. **Create new todo** - Add a new todo with title, description, and priority
2. **View all todos** - Display formatted list of all todos
3. **View specific todo** - Show detailed information for a specific todo
4. **Update todo** - Modify title, description, or priority of existing todo
5. **Delete todo** - Remove a todo from the list
6. **Mark todo as completed/pending** - Change todo status
7. **Save todos to file** - Manually save current state
8. **Export todos to text file** - Create human-readable export
9. **Exit** - Save and quit the program

### File Operations

#### Automatic Persistence
- Todos are automatically loaded on program start
- Todos are automatically saved on program exit
- Default storage file: `todos.dat`

#### Manual Save
Use menu option 7 to manually save your todos at any time.

#### Export to Text
Use menu option 8 to export todos to a human-readable text file for sharing or printing.

#### Backup System
- Automatic backup creation before overwriting existing files
- Backup files are named with `.backup` extension

## API Reference

### Core Functions

#### Todo Management
```c
TodoList* todo_list_create(void);
void todo_list_destroy(TodoList* list);
int todo_create(TodoList* list, const char* title, const char* description, Priority priority);
void todo_read_all(const TodoList* list);
int todo_read_by_id(const TodoList* list, int id);
int todo_update(TodoList* list, int id, const char* title, const char* description, int priority);
int todo_delete(TodoList* list, int id);
int todo_complete(TodoList* list, int id);
int todo_mark_pending(TodoList* list, int id);
```

#### File Operations
```c
int save_todos_to_file(const TodoList* list, const char* filename);
int load_todos_from_file(TodoList* list, const char* filename);
int export_todos_to_text(const TodoList* list, const char* filename);
int create_backup(const char* filename);
```

## Example Usage

### Creating a Todo
```
Enter todo title: Finish project documentation
Enter todo description: Complete README and code comments
Priority levels:
1. Low
2. Medium  
3. High
Enter priority: 2
```

### Viewing All Todos
```
=== TODO LIST ===
ID   | Title                | Priority   | Status   | Created             | Updated            
-----|----------------------|------------|----------|---------------------|--------------------- 
1    | Finish project doc.. | Medium     | Pending  | 2025-09-21 10:30    | 2025-09-21 10:30
2    | Review code          | High       | Completed| 2025-09-21 09:15    | 2025-09-21 11:45

Total todos: 2
```

## Error Handling

The application includes comprehensive error handling for:
- Memory allocation failures
- File I/O errors
- Invalid user input
- Data validation
- Boundary checks

## Memory Management

- Dynamic memory allocation for todo list
- Proper cleanup on program exit
- Memory leak prevention
- Buffer overflow protection

## Limitations

- Maximum 1000 todos per list
- Title limited to 100 characters
- Description limited to 500 characters
- Binary file format (not human-readable)

