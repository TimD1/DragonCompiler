# file header
	.file	"testing/proc.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	setb
	.type	setb, @function
setb:
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

# assignment

# find nonlocal variable
	mov		rcx, rbp
	mov		rcx, QWORD PTR [rcx-16]
	# end nonlocal variable
	mov		QWORD PTR [rcx-24], r8

# function footer
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	setb, .-setb



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
	mov		r8, 2

# assignment
	mov		QWORD PTR [rbp-24], r8

# call procedure 'setb'
# push procedure parameters onto stack
	push 5
	call setb
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
