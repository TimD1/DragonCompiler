# file header
	.file	"testing/write2.p"
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
	sub		rsp, 24
	# end function header

# main code

# evaluate expression
	mov		r8, 1

# assignment
	mov		QWORD PTR [rbp-40], r8

# evaluate expression
	mov		r8, 2

# assignment
	mov		QWORD PTR [rbp-32], r8

# evaluate expression
	mov		r8, 3

# assignment
	mov		QWORD PTR [rbp-24], r8

# call 'write' using fprintf
	mov		r9, QWORD PTR [rbp-32]
	imul	r9, QWORD PTR [rbp-24]
	mov		r8, QWORD PTR [rbp-40]
	add		r8, r9
	sub		r8, QWORD PTR [rbp-32]
	mov		rdx, r8
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
