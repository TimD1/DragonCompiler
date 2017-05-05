# DragonCompiler
## PASCAL compiler implemented in C for CS445
--------------------------------------------

### Haiku
> Dragon Compiler,<br>
> which took a whole semester,<br>
> it breathes fire at last.<br>

### User Guide
PASCAL files can be compiled using the command `./compiler <file_name.p>`. The command `gcc <file_name.s>` will then convert the generated assembly file to an executable, which can be run with `./a.out`. 

### Status Report
At this point, *I think* my compiler has successfully implemented all of the required components of a compiler for this class. For more details regarding the specified requirements, please see `checklist.txt`. All test cases are located in the subdirectory `testing/`.

### Unique Feature
My compiler has an incredibly unique strategy to resolve potential `if then` `if then else` conflicts. Rather than specifying that each `else` belongs to the nearest `if`, I instead modified the keywords so that the two control flow statements are `if then` and `if than else`. This will reduce programmer confusion and make it immediately apparent to which `if` clause the `else` begins.

I also included a few additional control flow statements, such as `repeat ... until` and `do ... while`.

### Design Document
1. Register Usage:

```
	rax = return value, division
	rbx = for loops
	rcx = nonlocal variable access
	rsp = stack pointer
	rbp = base pointer
	rip = instruction pointer

	rdi = reserved for io use, temp regs
	rsi = reserved for io use, temp regs
	rdx = reserved for io use, division
	

	r8  = temp var (top of stack)
	r9  = temp var
	r10 = temp var	
	r11 = temp var
	r12 = temp var
	r13 = temp var
	r14 = temp var
	r15 = temp var (bottom of stack)
```

2. Stack Frame Contents:
```
   low memory addresses
            .
            .
            .
   -------------------
   |     temp_reg3   | 	<--- rsp
   -------------------
   |     temp_reg2   |
   -------------------
   |     temp_reg1   |
   -------------------
   |     local3      |
   -------------------
   |     local2      |
   -------------------
   |     local1      |
   -------------------
   |     parent      |  rbp - 16
   -------------------
   |     return_val  |  rbp - 8
   -------------------
   |     old_rbp     | 	<--- rbp
   -------------------
   |     old_rip     |	(automatically added/removed by call/ret)
   -------------------
   |     param3      |
   -------------------
   |     param2      |
   -------------------
   |     param1      |
   -------------------
            .
            .
            .
   high memory addresses
```
3. Register Stack
   
```
   -------------------  		the value of each register is its initial index
   |       r8        | 7		positive-index values are stored in reg array
   -------------------			register name is given by static name list
   |       r9        | 6
   -------------------
   |       r10       | 5
   -------------------
   |       r11       | 4
   -------------------
   |       r12       | 3
   -------------------
   |       r13       | 2
   -------------------
   |       r14       | 1
   -------------------
   |       r15       | 0
-------------------------		negative-index values stored in temp array
   |      temp1      | -1		index 0 of temp array is never used
   -------------------			names are calculated based on offset from rbp
   |      temp2      | -2		temp array can grow to arbitrary size
   -------------------
   |      temp3      | -3
   -------------------
            .
            .
            .
```

### Room for Improvement
Some potential extensions of my existing compiler are:
- adding support for strings or pointers
- generating code for arrays and floating point numbers
