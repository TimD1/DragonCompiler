# file header
	.file	"testing/func5.p"
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
	sub		rsp, 0
	# end function header

# evaluate 'write' arguments
	mov		r8, QWORD PTR [rbp+16]
	sub		r8, QWORD PTR [rbp+24]

# call 'write' using fprintf
	mov		rdx, r8
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf


# evaluate expression
	mov		r8, QWORD PTR [rbp+16]
	sub		r8, QWORD PTR [rbp+24]

# return statement
	mov		QWORD PTR [rbp-8], r8
	jmp		.L1

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	sub, .-sub



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
