	.file	"dereference.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax

	mov	QWORD PTR [rbp-40], 7
	lea	rax, [rbp-40]
	mov	QWORD PTR [rbp-48], rax
	lea	rax, [rbp-48]
	mov	QWORD PTR [rbp-32], rax
	mov	rax, QWORD PTR [rbp-32]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-24], rax
	mov	rax, QWORD PTR [rbp-48]
	mov	rax, QWORD PTR [rax]
	mov	rax, QWORD PTR [rax]
	mov	QWORD PTR [rbp-16], rax
	mov	eax, 0
	mov	rdx, QWORD PTR [rbp-8]
	xor	rdx, QWORD PTR fs:40
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
