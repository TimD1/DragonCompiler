# file header
	.file	"testing/func.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	display
	.type	display, @function
display:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r15, QWORD PTR [rbp]
	mov		r15, QWORD PTR [r15-16]
	push	r15
	sub		rsp, 0
	# end function header

# call 'write' using fprintf
	mov		rdx, QWORD PTR [rbp+16]
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf


# evaluate expression
	mov		r15, QWORD PTR [rbp+16]
	add		r15, 1

# return statement
	mov		QWORD PTR [rbp-8], r15
	jmp		.L1

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	display, .-display



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp

# set up stack frame
	push	0
	push	rbp
	sub		rsp, 16
	# end function header

# main code

# call function 'display'
	push 10
	call	display
	add		rsp, 8

# evaluate expression
	mov		r15, rax

# assignment
	mov		QWORD PTR [rbp-32], r15

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
