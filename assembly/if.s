	.file	"if.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"1\n"
.LC1:
	.string	"2\n"
.LC2:
	.string	"3\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32

	mov	DWORD PTR [rbp-20], 3
	mov	eax, DWORD PTR [rbp-20]
	mov	DWORD PTR [rbp-16], eax
	mov	eax, DWORD PTR [rbp-16]
	mov	DWORD PTR [rbp-12], eax
	mov	eax, DWORD PTR [rbp-12]
	mov	DWORD PTR [rbp-8], eax
	mov	eax, DWORD PTR [rbp-8]
	mov	DWORD PTR [rbp-4], eax

	cmp	DWORD PTR [rbp-4], 9
	jg	.L2
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 2
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC0
	call	fwrite

.L2:
	cmp	DWORD PTR [rbp-4], 5
	jle	.L3
	cmp	DWORD PTR [rbp-12], 9
	jg	.L3
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 2
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC1
	call	fwrite

.L3:
	cmp	DWORD PTR [rbp-4], 0
	jg	.L4
	cmp	DWORD PTR [rbp-12], 10
	jle	.L5
.L4:
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 2
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC2
	call	fwrite

.L5:
	cmp	DWORD PTR [rbp-4], 0
	jg	.L6
	cmp	DWORD PTR [rbp-12], 10
	jle	.L7
	cmp	DWORD PTR [rbp-8], 2
	jg	.L7
.L6:
	mov	rax, QWORD PTR stderr[rip]
	mov	rcx, rax
	mov	edx, 2
	mov	esi, 1
	mov	edi, OFFSET FLAT:.LC2
	call	fwrite
.L7:
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
