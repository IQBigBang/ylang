#  TODO List

Bugfixes:
- Minimize build warnings
- Refactor code for better readability and maintainability (?)
- ~~Get rid of redundant code (e.g. 'lastValType')~~
- ~~Unary minus~~
- ~~Numbers not correctly passed into IR~~
- ~~Comments not working~~
- ~~Optimizations segfaulting~~
- ~~Correct decimal separator handling~~
- Power operator (for numbers) - currently unhandled
- Assignments into members (e.g. x->y = 2)



Enhancements:
- ~~Improve function definition grammar~~
- ~~Local variables~~
- ~~Multistatement functions and return statement (_removed goal due to design changes_)~~
- ~~If statement~~
- ~~Make language more functional (_removed goal_)~~
- ~~Switch statement~~
- Multi-condition `case`s (in `switch`)
- Better `if` statements
- ~~Structs~~
- Syntax simplification
- In-place assign (`+=`, `-=` ...)
- ~~External and unmangled functions~~
- ~~More unary operations (`!`, ...) - Lexer.cpp:99~~
- ~~Boolean operators (`and` ,`or`)~~
- ~~Better error reporting~~
- ~~Strings~~
- ~~Line counting (for better errors)~~
- Improve keyword and special symbol handling in the lexer (partly finished)
- ~~Do expression (multiple statements wrapper)~~
- Make the language more "imperative" (partly finished)
- ~~Add a `pass` statement (to allow for empty blocks) - added empty blocks instead~~
- ~~Syntax changes~~
- Update to non-legacy pass manager - Visitor.cpp:170
- ~~`include` syntax (for multiple files)~~
- Add more system types (int, pointer)
- Add `assert` and other self-testing techniques
- Add `break` in `while`
- ~~Support for `if` without an `else` case (implicit empty block)~~
- Add `discard` expression (for returning Void)



New features:
- JIT compiler
- ~~Optimizer~~
- ~~While statement~~
- `for` statement
- Type casting
- `match` statement (simple pattern matching)
- **Generics**
- **Add tests!**
- __Documentation__
- Sum types
- ~~Pointers (_removed goal_)~~
- __Extend std library__
- Type inference
- ~~Operator overloading~~ (and perhaps more operators)
- ~~CLI~~
- ~~Extend CLI~~
- Cross compilation (custom target-triple)
- ~~Garbage collector~~
- ~~Write custom lexer nad parser~~
- ~~Minimal std integration~~
