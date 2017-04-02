	.file	"func.c"
	.intel_syntax noprefix
	.text
	

	.globl	increment
	.type	increment, @function
increment:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6

	mov	DWORD PTR [rbp-4], edi
	mov	eax, DWORD PTR [rbp-4]
	add	eax, 1
	
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	increment, .-increment
	
	
	.globl	complicated_operation
	.type	complicated_operation, @function
complicated_operation:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	DWORD PTR [rbp-20], r8d
	mov	DWORD PTR [rbp-24], r9d
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
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	complicated_operation, .-complicated_operation


	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	
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
	mov	r8d, DWORD PTR [rbp-24]
	mov	edi, DWORD PTR [rbp-20]
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	r9d, r8d
	mov	r8d, edi
	mov	edi, eax
	call	complicated_operation
	
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	
	
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
