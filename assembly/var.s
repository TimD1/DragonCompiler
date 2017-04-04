	.file	"var.c"
	.intel_syntax noprefix
	.comm	b,4,4
	.text
	
	
	.globl	add3
	.type	add3, @function
add3:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-20], edi
	mov	DWORD PTR [rbp-24], esi
	mov	DWORD PTR [rbp-4], 2
	mov	edx, DWORD PTR [rbp-20]
	mov	eax, DWORD PTR [rbp-24]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-4]
	add	eax, edx
	pop	rbp
	ret
	.size	add3, .-add3
	
	
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	DWORD PTR [rbp-4], 42
	mov	edx, DWORD PTR b[rip]
	mov	eax, DWORD PTR [rbp-4]
	mov	esi, edx
	mov	edi, eax
	call	add3
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	
	
	
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
