# BFASM
*BFASM* is a compiled, semi-high-level programming language that compiles directly to raw, functioning brainfuck. It has support for variables, arithmetic, I/O, comparison, and simple control flow. It gets its name from its assembly-like syntax. It is currently in version *v0.0*.

Licensing info can be found at `LICENSING.txt`. Source code can be found at `.../code`.

# Statements
Commands in BFASM are called *statements*. The table of statements and their functions are listed below.

Command | Function | Version added
:-- | :-- | :--
`set %x v` | Set the variable `%x` to the value provided. Variable names must start with `%`. This is currently the only command that takes a number literal as an argument. | v0.0
`add %x %y %z` | Compute `%x + %y` and store it in `%z`. | v0.0
`sub %x %y %z` | Compute `%x - %y` and store it in `%z`. | v0.0
`mul %x %y %z` | Compute `%x * %y` and store it in `%z`. | v0.0
`div %x %y %z` | Compute `%x / %y` and store it in `%z`, rounding down if necessary. | v0.0
`out %x` | Output the ASCII character stored in `%x`. | v0.0
`inp %x` | Input an ASCII character store it in `%x`. | v0.0
`cmp %x %y %z %w %v` | Compare `%x` to `%y`. If `%x = %y`, set `%z` to `1`. If `%x > %y`, set `%w` to `1`. If `%x < %y`, set `%v` to `1`. All other arguments are set to zero. | v0.0
`begin_if %x` | If `%x` is nonzero, execute the code until the matching end_if (see next row) once, else skip it. | v0.0
`end_if` | Close the matching `begin_if`. | v0.0
`begin_while %x` | If `%x` is nonzero, execute the code until the matching end_if (see next row) while `%x` is nonzero, else skip it. | v0.0
`end_while %x` | Close the matching `begin_while`. The variable given must match the variable given to matching `begin_while`. | v0.0

# Brainfuck Equivalence
Pointer movement is handled by the compiler, and is handled differently for every program. Because of this, *exact* equivalences cannot be given, so only the computation code will be provided instead, leaving pointer arithmetic out.
Command | Function
:-- | :--
`set %x v` | `+++...`
`add %x %y %z` | `[-<+>]`
`sub %x %y %z` | `[-<+>]`
`mul %x %y %z` | `[>[->+>+<<]>>[<<+>>-]<<<-]>[-]<`
`div %x %y %z` | `[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>[-]`
`out %x` | `.`
`inp %x` | `,`
`cmp %x %y %z %w %v` | `+[>>>>[-<]<<]<[->>+<]>>[<<<->+>>[-]]>[-]<<<<`
`begin_if %x` | `[`
`end_if` | `]`
`begin_while %x` | `[`
`end_while %x` | `]`

# Examples
The following programs were compiled using BFASM. Each row of brainfuck code is 100 characters.

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
Generated brainfuck:
```
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]+++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..[-]++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++.[-]++++++++++++++++++++++++++++++++++++++++++++.[-]++++++++++++++++++++++++++++++++.[-]+++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.[-]+
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++.[-]+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++.[-]+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++.[-]+++++++++++++++++++++++++++++++++.
```
Output:
```
Hello, World!
```

## Cat
```
set %t 1
set %i 0
begin_while %t
inp %i
out %i
end_while %t
```
Generated brainfuck:
```
+[>,.<]
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
set %h 0
set %t 0
set %o 0
set %d 100
div %v %d %h
mul %d %h %d
sub %v %d %v
set %d 10
div %v %d %t
mul %d %t %d
sub %v %d %v
add %v %o %o
set %d 48
add %h %d %h
add %t %d %t
add %o %d %o
out %h
out %t
out %o
```
Generated brainfuck:
```
,>[->+>+<<]>>[-<<+>>]<<<[->>>+>+<<<<]>>>>[-<<<<+>>>>]<[-<+>]<<[-]>[-<+>]>>>+++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<<<<[->>>>>+>+<<<<<<]>>>>
>>[-<<<<<<+>>>>>>]<<[->>+>+<<<]>>[-<<+>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>[-]<<<<<<<[-]>>>
>>>>>[-<<<<<<<<+>>>>>>>>]<<<<<[->+>+<<]>>[-<<+>>]<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<[>[->
+>+<<]>>[<<+>>-]<<<-]>[-]<<[-]>>>[-<<<+>>>]<<<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<[->>+>+<<
<]>>>[-<<<+>>>]<[-<->]<<<<<<[-]>>>>>[-<<<<<+>>>>>]<[-]++++++++++<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+
>>>>>>]<<[->>+>+<<<]>>[-<<+>>]<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>[-]>[-]>[-]<<<<<<[-]>>>>>>>[-<<<<<<
<+>>>>>>>]<<<<<[->+>+<<]>>[-<<+>>]<<<<[->>>>+>+<<<<<]>>>>>[-<<<<<+>>>>>]<<[>[->+>+<<]>>[<<+>>-]<<<-]
>[-]<<[-]>>>[-<<<+>>>]<<<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<[->>+>+<<<]>>>[-<<<+>>>]<[-<->
]<<<<<<[-]>>>>>[-<<<<<+>>>>>]<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>]<<<[->>>+>+<<<<]>>>>[-<<<<+
>>>>]<[-<+>]<<<[-]>>[-<<+>>]<[-]++++++++++++++++++++++++++++++++++++++++++++++++<<<[->>>>+>+<<<<<]>>
>>>[-<<<<<+>>>>>]<<[->>+>+<<<]>>>[-<<<+>>>]<[-<+>]<<<<<[-]>>>>[-<<<<+>>>>]<<<[->>>+>+<<<<]>>>>[-<<<<
+>>>>]<<[->>+>+<<<]>>>[-<<<+>>>]<[-<+>]<<<<[-]>>>[-<<<+>>>]<<[->>+>+<<<]>>>[-<<<+>>>]<<[->>+>+<<<]>>
>[-<<<+>>>]<[-<+>]<<<[-]>>[-<<+>>]<<<<.>.>.
```
Sample output:
```
>>> $
036
```








