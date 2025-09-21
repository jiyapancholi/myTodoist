# Makefile for Todo List Manager
# Author: Jiya Pancholi
# Date: 2025-09-21

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Iinclude
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
DATADIR = data

# Project name and files
PROJECT_NAME = todo_manager
SOURCES = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(INCDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Default target
all: $(BUILDDIR) $(PROJECT_NAME)

# Create build directory
$(BUILDDIR):
	mkdir $(BUILDDIR)

# Build the main executable
$(PROJECT_NAME): $(OBJECTS)
	@echo "Linking $(PROJECT_NAME)..."
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROJECT_NAME)
	@echo "Build completed successfully!"

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(PROJECT_NAME)
	@echo "Debug build completed!"

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: $(PROJECT_NAME)
	@echo "Release build completed!"

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(BUILDDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	@echo "Cleaning up..."
	$(RM) -r $(BUILDDIR) $(PROJECT_NAME) $(PROJECT_NAME).exe
	@echo "Clean completed!"

# Rebuild everything
rebuild: clean all

# Run the program after building
run: $(PROJECT_NAME)
	@echo "Running $(PROJECT_NAME)..."
	./$(PROJECT_NAME)

# Install (copy to /usr/local/bin on Unix systems)
install: $(PROJECT_NAME)
	@echo "Installing $(PROJECT_NAME) to /usr/local/bin..."
	sudo cp $(PROJECT_NAME) /usr/local/bin/
	@echo "Installation completed!"

# Uninstall
uninstall:
	@echo "Uninstalling $(PROJECT_NAME)..."
	sudo rm -f /usr/local/bin/$(PROJECT_NAME)
	@echo "Uninstallation completed!"

# Create distribution package
dist: clean
	@echo "Creating distribution package..."
	tar -czf $(PROJECT_NAME)-src.tar.gz $(SOURCES) $(HEADERS) Makefile README.md
	@echo "Distribution package created: $(PROJECT_NAME)-src.tar.gz"

# Check code style (requires indent)
style-check:
	@echo "Checking code style..."
	@for file in $(SOURCES) $(HEADERS); do \
		echo "Checking $$file..."; \
		indent -kr -i4 -ts4 -l80 -lc80 -cli4 -st $$file | diff -u $$file - || true; \
	done

# Format code (requires indent)
format:
	@echo "Formatting code..."
	@for file in $(SOURCES) $(HEADERS); do \
		echo "Formatting $$file..."; \
		indent -kr -i4 -ts4 -l80 -lc80 -cli4 $$file; \
		rm -f $$file~; \
	done
	@echo "Code formatting completed!"

# Static analysis (requires cppcheck)
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c99 $(SOURCES)

# Memory check (requires valgrind)
memcheck: debug
	@echo "Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJECT_NAME)

# Count lines of code
count:
	@echo "Counting lines of code..."
	@wc -l $(SOURCES) $(HEADERS) | tail -1

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release version"
	@echo "  clean     - Remove generated files"
	@echo "  rebuild   - Clean and build"
	@echo "  run       - Build and run the program"
	@echo "  install   - Install to /usr/local/bin (Unix only)"
	@echo "  uninstall - Remove from /usr/local/bin (Unix only)"
	@echo "  dist      - Create source distribution package"
	@echo "  format    - Format code using indent"
	@echo "  analyze   - Run static analysis using cppcheck"
	@echo "  memcheck  - Run memory check using valgrind"
	@echo "  count     - Count lines of code"
	@echo "  help      - Show this help message"

# Dependencies
main.o: main.c todo.h file_io.h
todo.o: todo.c todo.h
file_io.o: file_io.c file_io.h todo.h

# Declare phony targets
.PHONY: all debug release clean rebuild run install uninstall dist style-check format analyze memcheck count help