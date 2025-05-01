# Simple C-Style Compiler with Native Code Generation

A minimal compiler for a small C-like language, implemented in C++.  
It lexes, parses, builds an AST, interprets, and emits x86_64 machine code at runtime.  

---

## Table of Contents

0. **Note**
1. **Features**  
2. **Language Supported**  
3. **Prerequisites**  
4. **Building**  
5. **Usage**  
6. **Project Structure**  
7. **Design & Architecture**  
8. **Adding New Language Features**  
9. **Contributing**  
10. **License**  

---

## Note

Inspired by Simple Compiler by Bart Stander, Utah Tech University

## Features

- **Front‐end**  
  - Whitespace/comments skipping  
  - Two-character operators: `<<`, `+=`, `-=`, `++`, `--`, `**`  
  - Token categories: keywords (`int`, `void`, `main`, `cout`, `endl`), identifiers, integers, operators, punctuation  
- **Parser / AST**  
  - Recursive‐descent parser  
  - Statement types: declarations, assignments, compound (`+=`/`-=`), `CoutStatement`, `if`/`else`, `while`, `do … while`, `for`, `repeat` loops  
  - Expression grammar with correct precedence:  
    1. `**` (right‐associative exponent)  
    2. `*`, `/`  
    3. `+`, `-`  
    4. Relational (`<`, `<=`, `>`, `>=`, `==`, `!=`, `%`)  
    5. Logical `&&`, `||`  
- **Interpreter**  
  - AST‐driven `Interpret()` for rapid feedback  
- **Code Generator**  
  - Emits x86_64 machine code into an executable buffer via `mprotect`  
  - Custom instruction‐stream API (`PushValue`, `PopPopAddPush`, `Jump`, etc.)  
  - Direct `CALL` into the generated `main` function  
  - Inline print support (`cout <<`) via a built‐in Linux syscall routine  

---

## Language Supported

```c++
void main() {
    // variable declaration + initialization
    int x = 5;

    // for‐loop
    for (int i = 0; i < 5; i++) {
        cout << i << endl;
    }

    // do‐while loop
    do {
        cout << x << " ";
        x += 1;
    } while (x < 10);

    // Python-style power operator
    cout << 2 ** 3 << endl;     // prints 8

    // repeat‐loop (counted)
    repeat(3) {
        cout << "foo" << endl;
    }
}
```

- **Comments**: none supported yet—extend `ScannerClass` if desired.  
- **Identifiers**: letters, digits, underscores; cannot begin with digit.  
- **Integers**: decimal only.  

---

## Prerequisites

- A modern C++ compiler (g++, clang++) with C++11 support  
- POSIX/Linux (uses `mprotect` for executable heap)  
- GNU Make (or adapt `Makefile` for your build system)  

---

## Building

```bash
git clone https://github.com/yourusername/your-compiler.git
cd your-compiler
make        # compiles Scanner.cpp, Parser.cpp, Node.cpp, Instructions.cpp, etc.
```

This produces an executable `main`.  

---

## Usage

Compile and run:

```bash
./main test1.txt    # test1.txt contains your source code
```

Example `test1.txt`:

```c++
void main() {
    int x = 2;
    cout << x ** 3 << endl;  // -> 8
}
```

---

## Project Structure

```
/compiler
  ├── Scanner.h / Scanner.cpp       # Lexer: tokenizes input
  ├── Parser.h  / Parser.cpp        # Recursive‐descent parser
  ├── Node.h    / Node.cpp          # AST node classes, Interpret & Code
  ├── Instructions.h / Instructions.cpp  
  │     # Machine‐code emitter & exec
  ├── Symbol.h   # Simple symbol‐table for variables
  ├── Debug.h    # Logging macros (MSG)
  ├── Makefile
  └── test1.txt  # Sample input
```

---

## Design & Architecture

1. **Lexing** (`ScannerClass`)  
   - Peeks for multi‐char tokens (`<<`, `++`, `--`, `+=`, `-=` , `**`)  
   - Falls back to a state‐machine for identifiers/numbers  

2. **Parsing** (`ParserClass`)  
   - Token look‐ahead with `PeekNextToken()`  
   - `Match(expectedType)` consumes tokens or errors  
   - Grammar split into precedence levels (`Expression()`, `Relational()`, `PlusMinus()`, `TimesDivide()`, `Power()`, `Factor()`)

3. **AST** (`Node.cpp`)  
   - Each node implements `Interpret()`, `Code(…)`, `PrintTree()`  
   - Binary operators derive from `BinaryOperatorNode`  
   - Statement nodes derive from `StatementNode`  

4. **Code Generation** (`InstructionsClass`)  
   - Writes raw bytes into `mCode[]`, marks region executable  
   - Provides helpers for pushing/popping, arithmetic, branching  
   - Generates a print‐integer routine at startup, then emits user code  

---

## Adding New Language Features

1. **Lexer**: extend `ScannerClass::GetNextToken()` to produce a new `TokenType` for your syntax.  
2. **Parser**: insert grammar in the proper precedence level, build new `Node` objects.  
3. **AST**: implement new `XXXNode` subclass in `Node.h/Node.cpp`:  
   - `Interpret()` for immediate semantics  
   - `CodeEvaluate()` (or `Code()`) to emit bytecodes  
4. **Testing**: add examples in `test*.txt` and verify both interpret and compiled paths.

---

## Contributing

1. Fork the repo & create a feature branch  
2. Add tests/examples in `test*.txt`  
3. Update `README.md` with any new requirements  
4. Submit a Pull Request—happy to review!  

---

## License

MIT License &mdash; see [LICENSE](LICENSE) for details.

---

*Built with ♥ by Spencer Ream*
