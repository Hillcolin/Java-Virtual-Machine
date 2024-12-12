# MiniJVM

MiniJVM is a minimalistic Java Virtual Machine emulator capable of executing bytecode from a custom `.mclass` file format. This project demonstrates fundamental concepts of virtual machines, including a stack-based architecture and basic bytecode operations.

---

## Features

- Reads and executes bytecode from a `.mclass` file.
- Supports common bytecode instructions such as:
  - Arithmetic operations (`IADD`, `ISUB`, `IMUL`, `IDIV`, `IREM`)
  - Stack manipulation (`PUSH`, `POP`, `DUP`)
  - Variable operations (`ILOAD`, `ISTORE`, `IINC`)
  - Control flow (`IFEQ`, `GOTO`, `RETURN`)
  - Printing (`PRINT`)
- Implements a basic operand stack and local variable storage.

---

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- `make` (optional, if creating a Makefile)

### Compilation

Compile the program using the following command:

```bash
gcc -o minijvm main.c stack.c minijvm.c
```

# MiniJVM Bytecode Instructions

MiniJVM supports the following bytecode instructions:

## Instructions

| Instruction  | Description                                                     |
|--------------|-----------------------------------------------------------------|
| `INST_ICONST0` | Pushes the integer `0` onto the stack.                          |
| `INST_POP`     | Pops the top item off the stack.                                |
| `INST_DUP`     | Duplicates the top item on the stack.                           |
| `INST_IADD`    | Pops and adds the top two items on the stack.                   |
| `INST_ISUB`    | Pops and subtracts the top two items on the stack.              |
| `INST_IMUL`    | Pops and multiplies the top two items on the stack.             |
| `INST_IDIV`    | Pops and divides the top two items on the stack.                |
| `INST_IREM`    | Pops and computes the remainder of the top two items.           |
| `INST_PRINT`   | Prints the top item on the stack.                               |
| `INST_BIPUSH`  | Pushes a signed byte onto the stack.                            |
| `INST_ILOAD`   | Loads a local variable onto the stack.                          |
| `INST_ISTORE`  | Stores the top of the stack into a local variable.              |
| `INST_IINC`    | Increments a local variable.                                   |
| `INST_IFEQ`    | Branches if the top of the stack equals `0`.                    |
| `INST_GOTO`    | Jumps to a specified offset in the bytecode.                    |
| `INST_RETURN`  | Ends the program.                                              |

### Example `.mclass` File

Let's create a sample `.mclass` file with the following bytecode:

```plaintext
BIPUSH 5
BIPUSH 3
IADD
PRINT
RETURN
```
### Bytecode Operations

This bytecode performs the following operations:

1. **Pushes the value `5` onto the stack.**
2. **Pushes the value `3` onto the stack.**
3. **Adds the top two items on the stack** (5 + 3).
4. **Prints the top item on the stack** (8).
5. **Ends the program.**



