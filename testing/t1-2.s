# file header
	.file	"testing/t1-2.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	boo
	.type	boo, @function
boo:
	push	rbp
	mov		rbp, rsp


# set up stack frame
	push	0
	mov		r15, QWORD PTR [rbp]
	mov		r15, QWORD PTR [r15-16]
	push	r15
	sub		rsp, 8
	# end function header

# evaluate expression
	mov		r15, 1.230000

# assignment
	mov		QWORD PTR [rbp-24], r15

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	boo, .-boo



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

# call procedure 'boo'
	push QWORD PTR [rbp-24]
	call	boo
	add		rsp, 8

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
