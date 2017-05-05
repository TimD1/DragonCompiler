# file header
	.file	"testing/func3.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	sub
	.type	sub, @function
sub:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r8, QWORD PTR [rbp]
	mov		r8, QWORD PTR [r8-16]
	push	r8
	sub		rsp, 8
	# end function header

# evaluate expression
	mov		r8, QWORD PTR [rbp+16]
	sub		r8, QWORD PTR [rbp+24]

# assignment
	mov		QWORD PTR [rbp-24], r8

# evaluate 'write' arguments
	mov		r8, QWORD PTR [rbp-24]

# call 'write' using fprintf
	mov		rdx, r8
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf


# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	sub, .-sub



# function header
	.globl	add
	.type	add, @function
add:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	push	[rbp]
	sub		rsp, 16
	# end function header

# evaluate expression
	mov		r8, QWORD PTR [rbp+16]
	add		r8, QWORD PTR [rbp+24]

# return statement
	mov		QWORD PTR [rbp-8], r8
	jmp		.L2

# function footer
.L2:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	add, .-add



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp

# set up stack frame
	push	0
	push	rbp
	sub		rsp, 0
	# end function header

# main code

# evaluate 'add' arguments
	mov		r8, 4
	push r8
	mov		r8, 2
	push r8

# call function 'add'
	call	add
	add		rsp, 16

# evaluate 'sub' arguments
	mov		r8, 3
	push r8
	mov		r8, rax
	push r8

# call procedure 'sub'
	call	sub
	add		rsp, 16

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
