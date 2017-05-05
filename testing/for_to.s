# file header
	.file	"testing/for_to.p"
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

# start for
	mov		rcx, 1
	mov		QWORD PTR [rbp-24], rcx
	# evaluated start position
	mov		rcx, 5
	mov		rbx, rcx
	# evaluated end position
.L1:
	cmp		QWORD PTR [rbp-24], rbx
	jg .L2
	# end for

# start do
# call 'write' using fprintf
	mov		rdx, ???
	mov		rax, QWORD PTR stderr[rip]
	mov		esi, OFFSET FLAT:.LC1
	mov		rdi, rax
	mov		eax, 0
	call	fprintf

	inc		QWORD PTR [rbp-24]
	jmp .L1
.L2:
	# end for-do

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
