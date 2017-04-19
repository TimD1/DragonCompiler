	.file	"printing.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16


	mov	rdx, 123
	mov	esi, OFFSET FLAT:.LC0
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	fprintf
	
	
	mov	eax, 0
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
