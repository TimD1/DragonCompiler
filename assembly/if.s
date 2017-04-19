	.file	"if.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%lld"
.LC1:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax
	mov	rax, QWORD PTR stdin[rip]
	lea	rdx, [rbp-24]
	mov	esi, OFFSET FLAT:.LC0
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_fscanf

	mov	rax, QWORD PTR [rbp-24]
	cmp	rax, 4
	setle	al
	movzx	eax, al
	mov	QWORD PTR [rbp-16], rax
	cmp	QWORD PTR [rbp-16], 0
	je	.L2

	mov	rdx, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR stderr[rip]
	mov	esi, OFFSET FLAT:.LC1
	mov	rdi, rax
	mov	eax, 0
	call	fprintf
.L2:
	mov	eax, 0
	call	__stack_chk_fail
.L4:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
