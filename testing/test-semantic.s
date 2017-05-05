# file header
	.file	"testing/test-semantic.p"
	.intel_syntax noprefix


# create io format strings
	.section	.rodata
.LC0: # reading
	.string "%lld"
.LC1: # writing
	.string "%lld\n"
	.text



# function header
	.globl	boo
	.type	boo, @function
boo:
	push	rbp
	mov		rbp, rsp


# function footer
	pop		rbp
	ret
	.size	boo, .-boo



# function header
	.globl	foo
	.type	foo, @function
foo:
	push	rbp
	mov		rbp, rsp


# evaluate expression
	mov		rcx, QWORD PTR [rbp-24]
	add		rcx, QWORD PTR [rbp-0]

# assignment
	mov		QWORD PTR [rbp-32], rcx

# function footer
	pop		rbp
	ret
	.size	foo, .-foo



# function header
	.globl	bar
	.type	bar, @function
bar:
	push	rbp
	mov		rbp, rsp


# evaluate expression
	mov		rcx, QWORD PTR [rbp-0]

# assignment
	mov		???, rcx

# function footer
	pop		rbp
	ret
	.size	bar, .-bar



# function header
	.globl	moo
	.type	moo, @function
moo:
	push	rbp
	mov		rbp, rsp


# start if-else
	mov		rcx, QWORD PTR [rbp-8]
	cmp		rcx, 0
	sete	cl
	movzx	rcx, cl
	cmp		rcx, 0
	je .L5
	# end conditional

# evaluate expression
	mov		rcx, 1

# assignment
	mov		QWORD PTR [rbp-16], rcx

# mid if-else
	jmp .L6
.L5:

# evaluate expression
	mov		rcx, 0

# assignment
	mov		QWORD PTR [rbp-16], rcx
.L6:

# end if

# function footer
	pop		rbp
	ret
	.size	moo, .-moo



# main header
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 256

# main code

# evaluate expression
	mov		r8, QWORD PTR [rbp-24]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl
	cmp		rcx, 0.001000
	ERROR	cl
	movzx	rcx, cl
	cmp		rcx, QWORD PTR [rbp-24]
	ERROR	cl
	movzx	rcx, cl
	mov		r8, QWORD PTR [rbp-72]
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	imul	r8, 1
	mov		rcx, QWORD PTR [rbp-32]
	add		rcx, r8
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	cmp		r8, 2.300000
	ERROR	r8b
	movzx	r8, r8b
	cmp		r8, QWORD PTR [rbp-32]
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-72]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl

# assignment
	mov		QWORD PTR [rbp-24], rcx

# evaluate expression
	mov		rcx, QWORD PTR [rbp-56]
	cmp		rcx, QWORD PTR [rbp-24]
	ERROR	cl
	movzx	rcx, cl
	imul	rcx, 45
	mov		r8, QWORD PTR [rbp-32]
	add		r8, rcx
	mov		rcx, QWORD PTR [rbp-56]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl

# assignment
	mov		QWORD PTR [rbp-24], rcx

# evaluate expression
	mov		r8, QWORD PTR [rbp-56]
	cmp		r8, 3
	ERROR	r8b
	movzx	r8, r8b
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl
	cmp		rcx, QWORD PTR [rbp-40]
	ERROR	cl
	movzx	rcx, cl
	cmp		rcx, QWORD PTR [rbp-24]
	ERROR	cl
	movzx	rcx, cl
	mov		r8, QWORD PTR [rbp-72]
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-56]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl
	mov		r8, QWORD PTR [rbp-64]
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, 2.300000
	add		rcx, r8
	mov		r8, QWORD PTR [rbp-64]
	cmp		r8, 2
	ERROR	r8b
	movzx	r8, r8b
	mov		r10, QWORD PTR [rbp-56]
	cmp		r10, 1
	ERROR	r10b
	movzx	r10, r10b
	cmp		r9, r10
	ERROR	r9b
	movzx	r9, r9b
	cmp		r9, r8
	ERROR	r9b
	movzx	r9, r9b
	cmp		r9, QWORD PTR [rbp-32]
	ERROR	r9b
	movzx	r9, r9b
	mov		r8, QWORD PTR [rbp-72]
	cmp		r8, r9
	ERROR	r8b
	movzx	r8, r8b
	mov		r9, QWORD PTR [rbp-24]
	add		r9, r8
	mov		r8, QWORD PTR [rbp-56]
	cmp		r8, r9
	ERROR	r8b
	movzx	r8, r8b
	imul	r8, 1
	mov		r9, QWORD PTR [rbp-32]
	add		r9, r8
	cmp		r8, r9
	ERROR	r8b
	movzx	r8, r8b
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	cmp		r8, 3
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-72]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl

# assignment
	mov		QWORD PTR [rbp-24], rcx

# start if
	mov		r8, QWORD PTR [rbp-56]
	cmp		r8, QWORD PTR [rbp-32]
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-64]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl
	mov		r8, QWORD PTR [rbp-48]
	cmp		r8, rcx
	setg	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-24]
	cmp		r9, rcx
	ERROR	r9b
	movzx	r9, r9b
	mov		rcx, QWORD PTR [rbp-88]
	cmp		rcx, r9
	ERROR	cl
	movzx	rcx, cl
	mov		r9, 7
	add		r9, rcx
	mov		rcx, QWORD PTR [rbp-56]
	cmp		rcx, QWORD PTR [rbp-32]
	ERROR	cl
	movzx	rcx, cl
	imul	rcx, 6
	cmp		rcx, r9
	setl	cl
	movzx	rcx, cl
	and		rcx, r8
	cmp		rcx, 0
	je .L7
	# end conditional

# evaluate expression
	mov		r8, QWORD PTR [rbp-24]
	sub		r8, 1
	mov		rcx, QWORD PTR [rbp-56]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl
	cmp		r8, rcx
	ERROR	r8b
	movzx	r8, r8b
	mov		rcx, QWORD PTR [rbp-88]
	cmp		rcx, r8
	ERROR	cl
	movzx	rcx, cl

# assignment
	mov		???, rcx
.L7:

# end if

# evaluate expression
	mov		rcx, 10.000000

# assignment
	mov		QWORD PTR [rbp-40], rcx

# evaluate expression
	mov		rcx, QWORD PTR [rbp-48]
	add		rcx, 0.000100

# assignment
	mov		QWORD PTR [rbp-48], rcx

# main footer
	mov		eax, 0
	leave
	ret

	.size	main, .-main



# file footer
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"

	.section	.note.GNU-stack,"",@progbits
