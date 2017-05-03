	.file	"return.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-8], 3
	mov	DWORD PTR [rbp-4], 5
	mov	eax, DWORD PTR [rbp-8]
	cmp	eax, DWORD PTR [rbp-4]
	jle	.L2
	mov	eax, 1
	jmp	.L3
.L2:
	mov	eax, -1
.L3:
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
