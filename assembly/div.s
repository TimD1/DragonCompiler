	.file	"div.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov	QWORD PTR [rbp-16], 30
	mov	QWORD PTR [rbp-8], 6

	mov	rax, QWORD PTR [rbp-16]
	cqo
	idiv	QWORD PTR [rbp-8]
	mov	QWORD PTR [rbp-16], rax

	mov	eax, 0
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
