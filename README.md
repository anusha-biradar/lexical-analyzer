# Lexical Analyzer

A C-based project that reads source code, identifies tokens and performs basic lexical validations.

## What this project does

The program reads a C source file and analyzes the different elements present in it.

It identifies tokens such as:

* Keywords
* Identifiers
* Operators
* Constants
* Brackets and braces
* Special symbols

It also performs validations for different types of input and syntax-related errors.

## Features

* Reads source code from a file
* Identifies different types of tokens
* Recognizes keywords and identifiers
* Handles relational and other operators
* Validates binary, octal, hexadecimal and decimal values
* Checks matching curly braces and parentheses
* Checks square brackets
* Reports invalid input

## Concepts Used

* C programming
* File handling
* Arrays
* Strings
* Structures
* Functions
* Tokenization
* Input validation
* Basic parsing

## Project Structure

```text
lexical-analyzer/
│
├── main.c
├── lexer.c
├── lexer.h
├── input.c
└── README.md
```

### File Description

| File      | Purpose                                                 |
| --------- | ------------------------------------------------------- |
| `main.c`  | Handles the main program flow                           |
| `lexer.c` | Contains the main lexical analysis and validation logic |
| `lexer.h` | Contains declarations used by the lexer                 |
| `input.c` | Contains the source-code input used for testing         |

## How it works

The program takes source code as input and processes it character by character.

The lexer checks the input and separates it into different tokens. It also performs validations while processing the source code and reports errors when invalid patterns are found.

## How to Compile

Compile the project using:

```bash
gcc main.c lexer.c -o lexer
```

Then run:

```bash
./lexer
```

## What I Learned

This project helped me understand how a compiler starts processing source code at the lexical level.

It gave me practical experience with tokenization, file handling, character-by-character processing and input validation in C.
