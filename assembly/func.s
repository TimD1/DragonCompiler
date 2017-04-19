	.file	"func.c"
	.intel_syntax noprefix
	.text

	.globl	increment
	.type	increment, @function
increment:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	eax, DWORD PTR [rbp-4]
	add	eax, 1
	pop	rbp
	ret
	.size	increment, .-increment


	.globl	complicated_operation
	.type	complicated_operation, @function
complicated_operation:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	DWORD PTR [rbp-20], r8
	mov	DWORD PTR [rbp-24], r9
	mov	edx, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-12]
	lea	ecx, [rdx+rax]
	mov	eax, DWORD PTR [rbp-16]
	imul	eax, DWORD PTR [rbp-20]
	mov	edx, eax
	mov	eax, DWORD PTR [rbp-24]
	add	eax, edx
	sub	ecx, eax
	mov	eax, ecx
	sub	DWORD PTR [rbp-4], eax
	mov	eax, DWORD PTR [rbp-4]
	pop	rbp
	ret
	.size	complicated_operation, .-complicated_operation


	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	DWORD PTR [rbp-24], 2
	mov	eax, DWORD PTR [rbp-24]
	mov	DWORD PTR [rbp-20], eax
	mov	eax, DWORD PTR [rbp-20]
	mov	DWORD PTR [rbp-16], eax
	mov	eax, DWORD PTR [rbp-16]
	mov	DWORD PTR [rbp-12], eax
	mov	eax, DWORD PTR [rbp-12]
	mov	DWORD PTR [rbp-8], eax
	mov	eax, DWORD PTR [rbp-8]
	mov	DWORD PTR [rbp-4], eax
	mov	eax, DWORD PTR [rbp-4]
	mov	edi, eax
	call	increment
	mov	r8, DWORD PTR [rbp-24]
	mov	edi, DWORD PTR [rbp-20]
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	r9, r8
	mov	r8, edi
	mov	edi, eax
	call	complicated_operation
	mov	eax, 0
	leave
	ret
	.size	main, .-main

	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
