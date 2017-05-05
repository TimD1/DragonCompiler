# file header
	.file	"testing/expr.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp

# set up stack frame
	push	0
	push	rbp
	sub		rsp, 48
	# end function header

# main code

# call 'read' using fscanf
	mov		rax, QWORD PTR fs:40
	xor		eax, eax
	mov		rax, QWORD PTR stdin[rip]
	lea		rdx, QWORD PTR [rbp-40]
	mov		esi, OFFSET FLAT:.LC0
	mov		rdi, rax
	mov		eax, 0
	call	__isoc99_fscanf


# evaluate expression
	mov		r15, 1

# assignment
	mov		QWORD PTR [rbp-32], r15

# evaluate expression
	mov		r15, 1

# assignment
	mov		QWORD PTR [rbp-24], r15

# evaluate expression
	mov		r15, 2

# assignment
	mov		QWORD PTR [rbp-64], r15

# evaluate expression
	mov		r15, 3

# assignment
	mov		QWORD PTR [rbp-56], r15

# evaluate expression
	mov		r15, 4

# assignment
	mov		QWORD PTR [rbp-48], r15

# evaluate expression
	mov		r15, QWORD PTR [rbp-64]
	imul	r15, QWORD PTR [rbp-56]
	add		r15, QWORD PTR [rbp-48]
	mov		rdi, QWORD PTR [rbp-72]
	mov		rsi, QWORD PTR [rbp-32]
	mov		rdi, rsi
	mov		QWORD PTR [rbp-72], rdi
	mov		QWORD PTR [rbp-32], rsi
	mov		rdi, QWORD PTR [rbp-72]
	mov		rsi, QWORD PTR [rbp-24]
	add		rdi, rsi
	mov		QWORD PTR [rbp-72], rdi
	mov		QWORD PTR [rbp-24], rsi
	sub		QWORD PTR [rbp-72], r15
	mov		r15, QWORD PTR [rbp-40]
	sub		r15, QWORD PTR [rbp-72]

# assignment
	mov		QWORD PTR [rbp-40], r15

# call 'write' using fprintf
	mov		rdx, QWORD PTR [rbp-40]
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf


# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
