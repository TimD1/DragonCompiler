	.file	"printing.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"Hello World 1\n"
.LC1:
	.string	"Hello World 3\n"
.LC2:
	.string	"Hello World %d"
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
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 14
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC0
	call	fwrite
	movabs	rax, 8022916924116329800
	mov	QWORD PTR [rbp-32], rax
	movabs	rax, 11210408094834
	mov	QWORD PTR [rbp-24], rax
	mov	DWORD PTR [rbp-16], 0
	mov	rdx, QWORD PTR stderr[rip]
	lea	rax, [rbp-32]
	mov	rsi, rdx
	mov	rdi, rax
	call	fputs
	mov	rax, QWORD PTR stdout[rip]
	mov	rcx, rax
	mov	edx, 14
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC1
	call	fwrite
	mov	rax, QWORD PTR stderr[rip]
	mov	edx, 4
	mov	esi, OFFSET FLAT:.LC2
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
