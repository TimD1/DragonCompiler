	.file	"printing.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%c\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16

	mov	BYTE PTR [rbp-1], 99
	movsx	edx, BYTE PTR [rbp-1]
	mov	rax, QWORD PTR stderr[rip]
	mov	esi, OFFSET FLAT:.LC0
	mov	rdi, rax
	mov	eax, 0
	call	fprintf

	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
