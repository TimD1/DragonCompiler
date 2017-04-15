	.file	"arg_passing.c"
	.intel_syntax noprefix
	.text
	
	.globl	add2
	.type	add2, @function
add2:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	edx, DWORD PTR [rbp-4]
	mov	eax, DWORD PTR [rbp-8]
	add	eax, edx
	pop	rbp
	ret
	.size	add2, .-add2
	
	.globl	add4
	.type	add4, @function
add4:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	edx, DWORD PTR [rbp-4]
	mov	eax, DWORD PTR [rbp-8]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-12]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-16]
	add	eax, edx
	pop	rbp
	ret
	.size	add4, .-add4
	
	.globl	add6
	.type	add6, @function
add6:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	DWORD PTR [rbp-20], r8d
	mov	DWORD PTR [rbp-24], r9d
	mov	edx, DWORD PTR [rbp-4]
	mov	eax, DWORD PTR [rbp-8]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-12]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-16]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-20]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-24]
	add	eax, edx
	pop	rbp
	ret
	.size	add6, .-add6

	.globl	add8
	.type	add8, @function
add8:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	DWORD PTR [rbp-20], r8d
	mov	DWORD PTR [rbp-24], r9d
	mov	edx, DWORD PTR [rbp-4]
	mov	eax, DWORD PTR [rbp-8]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-12]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-16]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-20]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-24]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+16]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+24]
	add	eax, edx
	pop	rbp
	ret
	.size	add8, .-add8

	.globl	add10
	.type	add10, @function
add10:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR [rbp-4], edi
	mov	DWORD PTR [rbp-8], esi
	mov	DWORD PTR [rbp-12], edx
	mov	DWORD PTR [rbp-16], ecx
	mov	DWORD PTR [rbp-20], r8d
	mov	DWORD PTR [rbp-24], r9d
	mov	edx, DWORD PTR [rbp-4]
	mov	eax, DWORD PTR [rbp-8]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-12]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-16]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-20]
	add	edx, eax
	mov	eax, DWORD PTR [rbp-24]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+16]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+24]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+32]
	add	edx, eax
	mov	eax, DWORD PTR [rbp+40]
	add	eax, edx
	pop	rbp
	ret
	.size	add10, .-add10
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	
	sub	rsp, 48
	
	mov	DWORD PTR [rbp-40], 1
	mov	eax, DWORD PTR [rbp-40]
	mov	DWORD PTR [rbp-36], eax
	mov	eax, DWORD PTR [rbp-36]
	mov	DWORD PTR [rbp-32], eax
	mov	eax, DWORD PTR [rbp-32]
	mov	DWORD PTR [rbp-28], eax
	mov	eax, DWORD PTR [rbp-28]
	mov	DWORD PTR [rbp-24], eax
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
	
	mov	edx, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	
	mov	esi, edx
	mov	edi, eax
	call	add2
	
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	edi, eax
	call	add4
	
	mov	r8d, DWORD PTR [rbp-24]
	mov	edi, DWORD PTR [rbp-20]
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	r9d, r8d
	mov	r8d, edi
	mov	edi, eax
	call	add6
	
	mov	r9d, DWORD PTR [rbp-24]
	mov	r8d, DWORD PTR [rbp-20]
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	edi, DWORD PTR [rbp-32]
	push	rdi
	mov	edi, DWORD PTR [rbp-28]
	push	rdi
	mov	edi, eax
	call	add8
	
	add	rsp, 16
	mov	r9d, DWORD PTR [rbp-24]
	mov	r8d, DWORD PTR [rbp-20]
	mov	ecx, DWORD PTR [rbp-16]
	mov	edx, DWORD PTR [rbp-12]
	mov	esi, DWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rbp-4]
	mov	edi, DWORD PTR [rbp-40]
	push	rdi
	mov	edi, DWORD PTR [rbp-36]
	push	rdi
	mov	edi, DWORD PTR [rbp-32]
	push	rdi
	mov	edi, DWORD PTR [rbp-28]
	push	rdi
	mov	edi, eax
	call	add10
	
	add	rsp, 32
	
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
