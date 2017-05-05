# file header
	.file	"testing/scope2.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	increaseA
	.type	increaseA, @function
increaseA:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r15, QWORD PTR [rbp]
	mov		r15, QWORD PTR [r15-16]
	push	r15
	sub		rsp, 0
	# end function header

# evaluate expression
	mov		r15, QWORD PTR [rbp+16]

# find nonlocal variable
	mov		rcx, rbp
	mov		rcx, QWORD PTR [rcx-16]
	# end nonlocal variable
	add		r15, QWORD PTR [rcx-24]

# assignment

# find nonlocal variable
	mov		rcx, rbp
	mov		rcx, QWORD PTR [rcx-16]
	# end nonlocal variable
	mov		QWORD PTR [rcx-24], r15

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	increaseA, .-increaseA



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
	mov		r15, 8

# assignment
	mov		QWORD PTR [rbp-24], r15

# call procedure 'increaseA'
	push 5
	call	increaseA
	add		rsp, 8

# call 'write' using fprintf
	mov		rdx, QWORD PTR [rbp-24]
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
