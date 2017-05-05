# file header
	.file	"testing/if_else.p"
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

# call 'read' using fscanf
	mov		rax, QWORD PTR fs:40
	xor		eax, eax
	mov		rax, QWORD PTR stdin[rip]
	lea		rdx, [rbp-24]
	mov		esi, OFFSET FLAT:.LC0
	mov		rdi, rax
	mov		eax, 0
	call	__isoc99_fscanf


# start if-else
	mov		rcx, QWORD PTR [rbp-24]
	cmp		rcx, 5
	setl	cl
	movzx	rcx, cl
	cmp		rcx, 0
	je .L1
	# end conditional

# evaluate expression
	mov		rcx, 0

# assignment
	mov		QWORD PTR [rbp-24], rcx

# mid if-else
	jmp .L2
.L1:

# evaluate expression
	mov		rcx, 10

# assignment
	mov		QWORD PTR [rbp-24], rcx

# end if
.L2:

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
