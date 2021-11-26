## Important notes on BaseLang instructions and syntax

### Structure
Unline modern programming languages, BaseLang is designed to be a lightweight interpreted language. 
Micropython requires 256Kb ROM and 16Kb Ram. 
ATmega328P chip has 2kb ram and 32k rom.
ATmega2560 chip has 8kb ram and 256k rom.
ATTiny series have max 3k ram (ATtiny3224)

So I wanted to create a language with the least amount of memory.

BaseLang has series of Sub instructions. `main:` is the entrypoint for your program.

```
main:
---
```

each sub ends with three dash `---`.

### Memory

All variables are at the global context per program. You can set a variable in a sub and read it in another sub.

```
main:
set test 5
call print
---
print:
sprintln test
---
```

### Keywords (not statements)

----
| `main:` | Reserved entrypoint for a program. If you define more than one `main:` in a program, first occurance of the sub will be the entrypoint and other `main:` subs will not be reachable. |
| `call <sub-name>` | Jump the sub cursor to given sub. |
| `goto <Instruction index>`  | Jump to given index in the current sub |
----
