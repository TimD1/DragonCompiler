	.file	"testing/t1-3.p"
	.intel_syntax noprefix
	.text




	.globl	boo
	.type	boo, @function
boo:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov rbp, rsp
	.cfi_def_cfa_register 6

	mov x, rbx
	mov rbx, a


	pop rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	boo, .-boo



	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits