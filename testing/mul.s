# file header
	.file	"testing/mul.p"
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
	sub		rsp, 256

# main code

# evaluate expression
	mov		rcx, 30

# assignment
	mov		QWORD PTR [rbp-32], rcx

# evaluate expression
	mov		rcx, 6

# assignment
	mov		QWORD PTR [rbp-24], rcx

# evaluate expression
	mov		rcx, QWORD PTR [rbp-32]
	imul	rcx, QWORD PTR [rbp-24]

# assignment
	mov		QWORD PTR [rbp-32], rcx

# call 'write' using fprintf
	mov		rdx, QWORD PTR [rbp-32]
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
