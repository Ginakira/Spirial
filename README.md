# Spiral

An awesome interpreted language based on CPP & Antlr3.

## Build

```shell
cmake -S . -B build
cd build
cmake --build .
```

## Run

```shell
cd bin
./spiral input.spr
# the "input.spr" can be any file you want to run with Spiral interpreter
```

## Features

- Define variable `def`
- Selective structure `if` `else`
- Loop structure `while` `do while` `for`
- Print statement `print`
- Numeric calculation `+` `-` `*` `/` `%`
- Scoping capabilities `{}`
- Weakly typed system `int` `float` `string`
- Jump out `break` `continue` `return`
- Function (support closures) `function`

## Design

- Deterministic Finite Automaton (DFA)
- Extended Backus-Naur Form (EBNF)
- Responsibility chain
- Abstract factory
- Singleton
- Visitor Pattern

## Third-party libraries

- Antlr 3.4