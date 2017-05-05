# file header
	.file	"testing/empty.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	empty
	.type	empty, @function
empty:
	push	rbp
	mov		rbp, rsp

	sub		rsp, 16


# evaluate expression
	mov		rcx, QWORD PTR [rbp+8]
	add		rcx, 1

# assignment
	mov		QWORD PTR [rbp+8], rcx

# function footer
	mov		rsp, rbp
	pop		rbp
	ret
	.size	empty, .-empty



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 16


# main code

# call procedure 'empty'
# copy for procedure: variable -> stack
	push 2
	call empty
	add		rsp, 8

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
