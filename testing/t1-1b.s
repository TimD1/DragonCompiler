# file header
	.file	"testing/t1-1b.p"
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
	sub		rsp, 0
	# end function header

# function footer
.L1:
	mov		rax, QWORD PTR [rbp-8]
	mov		rsp, rbp
	pop		rbp
	ret
	.size	boo, .-boo


