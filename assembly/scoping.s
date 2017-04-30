	.file	"scoping.c"
	.intel_syntax noprefix
	.globl	c
	.data
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.long	7
	.text
	.globl	square
	.type	square, @function
square:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	eax, DWORD PTR [rbp-4]
	imul	eax, DWORD PTR [rbp-4]
	mov	edx, eax
	mov	eax, DWORD PTR c[rip]
	add	eax, edx
	pop	rbp
	ret
	.size	square, .-square
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR [rbp-4], 3
	mov	eax, DWORD PTR [rbp-4]
	mov	edi, eax
	call	square
	mov	DWORD PTR [rbp-4], eax
	mov	rax, QWORD PTR stderr[rip]
	mov	edx, DWORD PTR [rbp-4]
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
