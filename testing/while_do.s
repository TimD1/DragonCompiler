# file header
	.file	"testing/while_do.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 256

# main code

# evaluate expression
	mov		rcx, 0

# assignment
	mov		QWORD PTR [rbp-24], rcx

# start while
.L1:
	mov		rcx, QWORD PTR [rbp-24]
	cmp		rcx, 10
	setl	cl
	movzx	rcx, cl
	cmp		rcx, 0
	je .L2
	# end while, start do

# evaluate expression
	mov		rcx, QWORD PTR [rbp-24]
	add		rcx, 1

# assignment
	mov		QWORD PTR [rbp-24], rcx
jmp .L1
.L2:
	# end do

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
