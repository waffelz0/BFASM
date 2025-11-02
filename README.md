# BFASM
*BFASM* is a compiled, semi-high-level programming language that compiles directly to raw, functioning brainfuck. It has support for variables, arithmetic, I/O, comparison, and simple control flow. It gets its name from its assembly-like syntax. It is currently in version *v0.1*.

Licensing info can be found at `LICENSING.txt`. Source code can be found at `/code`.

To use BFASM, download and build the source code, then run the `.exe` file with the first argument being the file you want to compile.

Please report any bugs to me on discord via *@waffelz0212*.

# Statements
In BFASM, commands are called *statements*. The table of statements and their functions are listed below.

Statement | Function | Version added
:-- | :-- | :--
`set %x v` | Set the variable `%x` to the value provided. Variable names must start with `%`. This is currently the only statement that takes a number literal as an argument. | v0.0
`add %x %y %z` | Compute `%x + %y` and store it in `%z`. | v0.0
`sub %x %y %z` | Compute `%x - %y` and store it in `%z`. | v0.0
`mul %x %y %z` | Compute `%x * %y` and store it in `%z`. | v0.0
`div %x %y %z` | Compute `%x / %y` and store it in `%z`, rounding down if necessary. | v0.0
`mod %x %y %z` | Compute `%x % %y` and store it in `%z`. | v0.1
`out %x` | Output the ASCII character stored in `%x`. | v0.0
`inp %x` | Input an ASCII character and store it in `%x`. | v0.0
`cmp %x %y %z %w %v` | Compare `%x` to `%y`. If `%x = %y`, set `%z` to `1`. If `%x > %y`, set `%w` to `1`. If `%x < %y`, set `%v` to `1`. Any other output arguments are set to zero. | v0.0
`if %x` | If `%x` is nonzero, execute the code until the matching `end` statement once, else skip it. | v0.1
`while %x` | If `%x` is nonzero, execute the code until the matching `end` statement while `%x` is nonzero, else skip it. | v0.1
`end %x` | Close the matching `if`/`while` statement. | v0.1
`! ...` | Comment, ignored by the compiler. | v0.1

# Removed Statements
The table of statements that have been removed in earlier versions are listed below.

Statement | Reason for Removal | Version Removed
:-- | :-- | :--
`begin_if %x` | Replaced with `if`. | v0.1
`end_if` | Unified with `end_while` as `end`. | v0.1
`begin_while %x` | Replaced with `while`. | v0.1
`end_while %x` | Unified with `end_if` as `end`. | v0.1

# Brainfuck Equivalence
*Actual compiled code will contain many pointer adjustments that differ for every program.*

Statement | Brainfuck Equivalence
:-- | :--
`set %x v` | `+++...`
`add %x %y %z` | `[-<+>]`
`sub %x %y %z` | `[-<->]`
`mul %x %y %z` | `[>[->+>+<<]>>[<<+>>-]<<<-]>[-]<`
`div %x %y %z` | `[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>[-]`
`mod %x %y %z` | `[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>>[-]<`
`out %x` | `.`
`inp %x` | `,`
`cmp %x %y %z %w %v` | `+[>>>>[-<]<<]<[->>+<]>>[<<<->+>>[-]]>[-]<<<<`
`if %x` | `[`
`while %x` | `[`
`end` | `]`

# Examples
The following programs were compiled using BFASM. Brainfuck code is split by 100 instructions.

## Print "Hello, World!"
```
set %c 72
out %c
set %c 101
out %c
set %c 108
out %c
out %c
set %c 111
out %c
set %c 44
out %c
set %c 32
out %c
set %c 87
out %c
set %c 111
out %c
set %c 114
out %c
set %c 108
out %c
set %c 100
out %c
set %c 33
out %c
```

Generated brainfuck (1.068 kB):
```brainfuck
>++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]+++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..[-]+++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++.[-]++++++++++++++++++++++++++++++++++++++++++++.[-]++++++++++++++++++++++++++++++++.[-]++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]+++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++.[-]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++.[-]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++.[-]+++++++++++++++++++++++++++++++++.
```
Output:
```
Hello, World!
```

## Cat
```
set %t 1
set %i 0
while %t
inp %i
out %i
end
```

Generated brainfuck (8 B):
```brainfuck
>+[>,.<]
```

Sample output:
```
>>> h
h
>>> i
i
>>> 1
1
>>> 2
2
>>> 3
3
```

## Decimal Output
```
set %i 0
inp %i
set %v 0
add %v %i %v
set %d 10
set %ones 0
set %tens 0
set %hund 0
mod %v %d %ones
div %v %d %v
mod %v %d %tens
div %v %d %v
add %hund %v %hund
set %= 0
set %> 0
set %< 0
set %d 48
add %hund %d %hund
add %tens %d %tens
add %ones %d %ones
set %d 99
cmp %i %d %= %> %<
if %>
	out %hund
end
set %d 9
cmp %i %d %= %> %<
if %>
	out %tens
end

out %ones
```
Generated brainfuck (2.936 kB):
```brainfuck
>,>[->+>+<<]>>[-<<+>>]<<<[->>>+>+<<<<]>>>>[-<<<<+>>>>]<[-<+>]<<[-]>[-<+>]++++++++++<[->>>>>+>+<<<<<<
]>>>>>>[-<<<<<<+>>>>>>]<<<<<[->>>>>+>+<<<<<<]>>>>>[-<<<<<+>>>>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[
-]>[-]>>[-]<<<<<<<[-]>>>>>>[-<<<<<<+>>>>>>]<<<<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<<<<[->>>
>>+>+<<<<<<]>>>>>[-<<<<<+>>>>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>[-]<<<<<<<<[-]>>>>>>>>>[-<
<<<<<<<<+>>>>>>>>>]<<<<<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<<<<[->>>>>+>+<<<<<<]>>>>>[-<<<<
<+>>>>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>>[-]<<<<<<[-]>>>>>[-<<<<<+>>>>>]<<<<<<<<[->>>>>+>
+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<<<<[->>>>>+>+<<<<<<]>>>>>[-<<<<<+>>>>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<
<<<<]>[-]>[-]>[-]<<<<<<<<[-]>>>>>>>>>[-<<<<<<<<<+>>>>>>>>>]<<<<<[->+>+<<]>>[-<<+>>]<<<<<<[->>>>>>+>+
<<<<<<<]>>>>>>>[-<<<<<<<+>>>>>>>]<[-<+>]<<[-]>[-<+>]<<<<[-]+++++++++++++++++++++++++++++++++++++++++
+++++++>>>[->>>>+>+<<<<<]>>>>>[-<<<<<+>>>>>]<<<<<<<<[->>>>>>>>+>+<<<<<<<<<]>>>>>>>>>[-<<<<<<<<<+>>>>
>>>>>]<[-<+>]<<<<<[-]>>>>[-<<<<+>>>>]<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<<<<<<<[->>>>>>>>+
>+<<<<<<<<<]>>>>>>>>>[-<<<<<<<<<+>>>>>>>>>]<[-<+>]<<<<<<[-]>>>>>[-<<<<<+>>>>>]<<<<<<[->>>>>>+>+<<<<<
<<]>>>>>>>[-<<<<<<<+>>>>>>>]<<<<<<<<[->>>>>>>>+>+<<<<<<<<<]>>>>>>>>>[-<<<<<<<<<+>>>>>>>>>]<[-<+>]<<<
<<<<[-]>>>>>>[-<<<<<<+>>>>>>]<<<<<<<[-]+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++<<[->>>>>>>>>>>>+>+<<<<<<<<<<<<<]>>>>>>>>>>>>>[-<<<<<<<<<<<<<+
>>>>>>>>>>>>>]<<<<<<<<<<<[->>>>>>>>>>>+>+<<<<<<<<<<<<]>>>>>>>>>>>>[-<<<<<<<<<<<<+>>>>>>>>>>>>]<<<<<+
[>>>>[-<]<<]<[->>+<]>>[<<<->+>>[-]]>[-]<<<<<<<[-]>>>[-<<<+>>>]<<[-]>>>[-<<<+>>>]<<[-]>>>[-<<<+>>>]<<
<<<<<<<<<<[-]>>>>>>>>[->>+>+<<<]>>>[-<<<+>>>]<<<<<<<<<<<[->>>>>>>>>>>+>+<<<<<<<<<<<<]>>>>>>>>>>>>[-<
<<<<<<<<<<<+>>>>>>>>>>>>]<[-<+>]<<<<<<<<<<<[-]>>>>>>>>>>[-<<<<<<<<<<+>>>>>>>>>>]<<<<<<<<<<[>>>>>>.<<
<<<<[-]]>>>[-]+++++++++<<[->>>>>>>>>>>>+>+<<<<<<<<<<<<<]>>>>>>>>>>>>>[-<<<<<<<<<<<<<+>>>>>>>>>>>>>]<
<<<<<<<<<<[->>>>>>>>>>>+>+<<<<<<<<<<<<]>>>>>>>>>>>>[-<<<<<<<<<<<<+>>>>>>>>>>>>]<<<<<+[>>>>[-<]<<]<[-
>>+<]>>[<<<->+>>[-]]>[-]<<<<<<<[-]>>>[-<<<+>>>]<<[-]>>>[-<<<+>>>]<<[-]>>>[-<<<+>>>]<<<<<<<<<<<<[-]>>
>>>>>>[->>+>+<<<]>>>[-<<<+>>>]<<<<<<<<<<<[->>>>>>>>>>>+>+<<<<<<<<<<<<]>>>>>>>>>>>>[-<<<<<<<<<<<<+>>>
>>>>>>>>>]<[-<+>]<<<<<<<<<<<[-]>>>>>>>>>>[-<<<<<<<<<<+>>>>>>>>>>]<<<<<<<<<<[>>>>>.<<<<<[-]]>>>>.
```
Sample output:
```
>>> $
36
```

# Explanation
BFASM works by splitting memory into two chunks-- cells used by variables, and temporary cells used by operations. Temporary cells are always reset to 0 after every statement, which ensures they can be used smoothly for the next operation. It also reserves a single cell for a variable internally called `%_`. It is specifically used for if statements to prevent the values checked from being cleared.

The tape structure for a compiled BFASM program is similar to the diagram below.
```
 %_    Reserved for variables.    Reserved for operations.
 —^— —————————————^————————————— —————————————^—————————————
 [0] [_] [_] [_] [_] [_] ... [_] [0] [0] [0] [0] [0] [0] ...
```
Statements copy their operands to the first free cells (those in the second half of the tape), perform the operation, and move the result to the desired cell. Because of this, the majority of most generated programs are pointer arithmetic (`<` and `>`).

Since brainfuck has 8-bit cells, operations will silently overflow. This is not a bug.

# Changelog

## v0.1
- Added `mod` statement  
- Unified `end_if` and `end_while` as `end`  
- Renamed `begin_if` and `begin_while` to `if` and `while` respectively  
- Fixed a giant pointer arithmetic issue in `if` statements  
- Added comments with `! ...`  
- Vastly improved error checking

## v0.0
- Initial release

# Known Issues
- Defining a variable inside an if statement(?) or a while loop messes up the internal pointer, which can completely mess up brainfuck logic.  


