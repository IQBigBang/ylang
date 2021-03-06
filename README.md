## Ylang

**Ylang** is a toy language for my toy compiler. Even though this is just a side project, I'd still like the language to evolve into something usable in real life. If you find ugly code and want to fix it (or just tell me), a better way to do something or if you have experience in working with LLVM or if you are just interested, let me know!

### The language

**Ylang** is a statically-typed compiled (possibly JIT-compiled) language. It should be usable as a scripting language yet performant.

### Features

Currently, the language supports the following:

- Custom functions (together with name mangling to enable for function overloading)
- Local variables
- Operators (plus, minus, times, divide, power, comparison, negation)
- If expression and while statement
- Switch statement (partly)
- Custom type (struct-like) definition
- Built-in types `Num`, `Bool` and `Str`

For a list of features to be implemented, see `TODO.md` 

### Installation

If you'd want to (for whatever reason) try out the compiler yourself, use the following commands for installation:
```
sudo apt install make llvm-9 clang-9
git clone https://github.com/IQBigBang/ylang.git
cd ylang && make -j
```
The compiler requires the LLVM toolchain to be installed and has only been tested on Linux (but it might  work on Mac too).

### Example

```
include std

fn Num fact_rec(Num n) {
	if n == 1
		1
	else {
		fact_rec(n - 1) * n;
	}
}

fn Num fact_loop(Num n) {
	let res = 1;
	while n > 1 {
		res *= n;
		n -= 1;
	};
	res
}

fn Void main() { # Main function
	let rec_res = fact_rec(14);
	let loop_res = fact_loop(14);
	if rec_res != loop_res {
		print("Error! Got different results: ");
		print(rec_res);
		print(loop_res);
	} else {
		print("Got result: ");
		print(rec_res);
	}
}
```

