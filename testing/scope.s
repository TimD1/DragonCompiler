# file header
	.file	"testing/scope.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	sum
	.type	sum, @function
sum:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r8, QWORD PTR [rbp]
	mov		r8, QWORD PTR [r8-16]
	push	r8
	sub		rsp, 0
	# end function header

# evaluate expression
	mov		r8, QWORD PTR [rbp+16]

# find nonlocal variable
	mov		rcx, rbp
	mov		rcx, QWORD PTR [rcx-16]
	mov		rcx, QWORD PTR [rcx-24]
	# end nonlocal variable
	add		r8, rcx

# assignment
	mov		QWORD PTR [rbp+16], r8

# call 'write' using fprintf
	mov		rdx, QWORD PTR [rbp+16]
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf


# function footer
	mov		rsp, rbp
	pop		rbp
	ret
	.size	sum, .-sum



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

# evaluate expression
	mov		r8, 8

# assignment
	mov		QWORD PTR [rbp-24], r8

# call procedure 'sum'
# copy for procedure: variable -> stack
	push 5
	call sum
	add		rsp, 8

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
