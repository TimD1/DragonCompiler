# file header
	.file	"testing/recursion2.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	factorial
	.type	factorial, @function
factorial:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r8, QWORD PTR [rbp]
	mov		r8, QWORD PTR [r8-16]
	push	r8
	sub		rsp, 0
	# end function header

# start if-else
	mov		r8, QWORD PTR [rbp+16]
	cmp		r8, 1
	sete	r8b
	movzx	r8, r8b
	cmp		r8, 0
	je		.L2
	# end conditional

# evaluate expression
	mov		r8, QWORD PTR [rbp+16]

# return statement
	mov		QWORD PTR [rbp-8], r8
	jmp		.L1

# mid if-else
	jmp		.L3
.L2:

# evaluate 'factorial' arguments
	mov		r8, QWORD PTR [rbp+16]
	sub		r8, 1
	push r8

# call function 'factorial'
	call	factorial
	add		rsp, 8

# evaluate expression
	mov		r9, rax
	mov		r8, QWORD PTR [rbp+16]
	imul	r8, r9

# return statement
	mov		QWORD PTR [rbp-8], r8
	jmp		.L1
.L3:

# end if

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	factorial, .-factorial



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp

# set up stack frame
	push	0
	push	rbp
	sub		rsp, 8
	# end function header

# main code

# evaluate 'factorial' arguments
	mov		r8, 5
	push r8

# call function 'factorial'
	call	factorial
	add		rsp, 8

# evaluate expression
	mov		r8, rax

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


# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
