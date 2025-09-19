# TinySQL: SQL Lexer + LL(1) Parser in C++

A small SQL front-end that tokenizes and parses a subset of SQL using a hand-rolled lexer and a table-driven LL(1) predictive parser (no Lex/Yacc/Bison/Flex). It reads a script (`sample.sql`), builds FIRST/FOLLOW sets and a parse table from the grammar, and then validates the script with clear, line/column error messages.

## Features

- **Lexer**
  - Case-insensitive keywords: `SELECT, FROM, WHERE, INSERT, INTO, VALUES, ORDER, BY, GROUP, AND, OR, NOT, DELETE`
  - Tokens: identifiers, integer numbers, single-quoted strings, `= > < ( ) , ; *`
  - Tracks **line & column** for precise diagnostics
- **Grammar & Parser**
  - Subset of SQL: `SELECT`, `INSERT`, `DELETE`
  - `WHERE` with `OR`/`AND`/`NOT` and parentheses, `ORDER BY`, `GROUP BY`
  - Lists: attribute lists and value lists
  - Multiple statements per file (`;` terminated)
  - Computes **FIRST/FOLLOW** sets and builds an **LL(1) parse table**, then runs a **predictive parser** with a stack
- **Error handling**
  - Helpful messages (e.g., *missing comma between attributes*, *missing `VALUES`*, *missing table name*, unexpected EOF / missing `;`), all with line/column positions

## Supported Grammar (BNF → LL(1) form)
