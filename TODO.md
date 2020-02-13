#  TODO List

Bugfixes:
- Minimize build warnings
- Refactor code for better readability and maintainability (?)
- ~~Get rid of redundant code (e.g. 'lastValType')~~
- ~~Unary minus~~
- ~~Numbers not correctly passed into IR~~
- ~~Comments not working~~
- ~~Optimizations segfaulting~~
- Correct decimal separator handling - Lexer.cpp:137

Code improvements:
- 
- 

Enhancements:
- ~~Improve function definition grammar~~
- ~~Local variables~~
- ~~Multistatement functions and return statement (__removed goal due to design changes__)~~
- ~~If statement~~
- ~~Make language more functional (__removed goal__)~~
- ~~Switch statement~~
- Multi-condition `case`s (in `switch`)
- Better `if` statements
- ~~Structs~~
- Syntax simplification
- External and unmangled functions
- More unary operations (`!`, ...) - Lexer.cpp:99
- ~~Better error reporting~~
- ~~Strings~~
- ~~Line counting (for better errors)~~
- Improve keyword and special symbol handling in the lexer
- ~~Do expression (multiple statements wrapper)~~
- Make the language more "imperative" (partly finished)
- Add a `pass` statement (to allow for empty blocks)
- ~~Syntax changes~~
- Update to non-legacy pass manager - Visitor.cpp:170

New features:
- JIT compiler
- ~~Optimizer~~
- ~~While statement~~
- `for` statement
- Type casting
- `match` statement (simple pattern matching)
- Add tests!
- __Documentation__
- Sum types
- ~~Pointers (__removed goal__)~~
- Full std library
- Type inference
- Operator overloading (and perhaps more operators)
- ~~CLI~~
- Extend CLI
- ~~Garbage collector~~
- ~~Write custom lexer nad parser~~
- ~~Minimal std integration~~
