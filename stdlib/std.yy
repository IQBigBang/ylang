# Standard Library definitions
# Only for Public STL
# Private STL defintions are registered by the compiler (often because they use pointer types)

# Print a number
fn (external) Void print(Num n)

# Print a string
fn (external) Void print(Str s)

# Join two strings
fn (external) Str add(Str s1, Str s2)