# minishell
![minishell](https://img.shields.io/badge/Project-minishell-blue)

A simple shell implementation inspired by bash, developed as part of the 42 curriculum.

## Overview

`minishell` is a lightweight shell that replicates basic bash functionality. This project uses UNIX system calls to implement process creation, file manipulation, and signal handling in a custom shell interpreter.

## Features

- Command execution (built-in and external commands)
- Command chaining with pipes (`|`)
- Input/output redirection (`>`, `>>`, `<`)
- Here-document support (`<<`)
- Environment variable expansion (`$VAR`)
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- Path resolution (relative and absolute)
- Quote handling (single `'` and double `"` quotes)

## Getting Started

### Prerequisites

- GCC or Clang compiler
- GNU Make
- Readline library

### Installation

Clone the repository and compile the project:

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
```

### Usage

Run the shell:

```bash
./minishell
```

Execute a command directly:

```bash
./minishell -c "echo hello world"
```

## Supported Commands

### Built-in Commands

- `echo` - Display a line of text (with `-n` option support)
- `cd` - Change the current directory
- `pwd` - Print the current working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display environment variables
- `exit` - Exit the shell

### Shell Features

- **Pipes**: `cmd1 | cmd2`
- **Redirections**:
  - Output: `cmd > file`, `cmd >> file`
  - Input: `cmd < file`
  - Here-document: `cmd << DELIMITER`
- **Variable Expansion**: `$VAR`, `$?` (previous command's exit status)
- **Quote Handling**:
  - Single quotes (`'`): Treat all special characters literally
  - Double quotes (`"`): Allow variable expansion

## Project Structure

```
.
├── includes/            # Header files
├── srcs/
│   ├── builtins/        # Built-in command implementations
│   ├── executor/        # Command execution handlers
│   ├── gnl/             # Get_next_line implementation
│   ├── libft/           # Custom C standard library functions
│   ├── parser/          # Input parsing and tokenization
│   ├── signals/         # Signal handling
│   ├── utils/           # Utility functions
│   └── main.c           # Main entry point
├── Makefile             # Build configuration
└── mytester.sh          # Basic test script
```

## Testing

Run basic tests:

```bash
./mytester.sh
```

Check for memory leaks using valgrind:

```bash
./vshell
```

## Contributing

This project is an educational exercise. Feel free to fork and use for your own learning.

## License

This project is part of the 42 curriculum and follows its guidelines and licenses.