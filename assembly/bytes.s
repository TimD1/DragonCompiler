	.file	"bytes.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%c\n"
	.text
	.globl	main
	.type	main, @function
main:
	// setup stack
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rbp-8], rax
	xor	eax, eax



	mov	BYTE PTR [rbp-17], 99			// a = 'c'
	movzx	eax, BYTE PTR [rbp-17] 		
	sub	eax, 1							// a--
	
	mov	BYTE PTR [rbp-17], al			
	lea	rax, [rbp-17]
	movzx	eax, BYTE PTR [rax]
	add	eax, 1
	
	mov	edx, eax
	mov	rax, QWORD PTR [rbp-16]
	mov	BYTE PTR [rax], dl

	movzx	eax, BYTE PTR [rbp-17]
	movsx	edx, al
	mov	rax, QWORD PTR stderr[rip]
	mov	esi, OFFSET FLAT:.LC0
	mov	rdi, rax
	mov	eax, 0
	call	fprintf
	

	// return
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
