	.file	"scanning.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	
	lea	rdx, [rbp-12]
	mov	esi, OFFSET FLAT:.LC0
	mov	rdi, QWORD PTR stdin[rip]
	mov	eax, 0
	call	__isoc99_fscanf
	
	mov	edx, DWORD PTR [rbp-12]
	mov	rax, QWORD PTR stderr[rip]
	mov	esi, OFFSET FLAT:.LC1
	mov	rdi, rax
	mov	eax, 0
	call	fprintf

	mov	eax, 0
	mov	rcx, QWORD PTR [rbp-8]
	xor	rcx, QWORD PTR fs:40
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
