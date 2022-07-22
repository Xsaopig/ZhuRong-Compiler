	.arch armv7
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"test.c"
	.text
	.global	a
	.section	.rodata
	.align	2
	.type	a, %object
	.size	a, 80
a:
	.word	1
	.word	2
	.word	3
	.word	4
	.space	8
	.word	7
	.space	4
	.space	48
	.text
	.align	1
	.global	main
	.arch armv7
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	movw	r2, #:lower16:a
	movt	r2, #:upper16:a
	ldr	r3, [r7, #4]
	lsls	r3, r3, #3
	add	r3, r3, r2
	ldr	r3, [r3, #4]
	adds	r3, r3, #1
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Raspbian 10.2.1-6+rpi1) 10.2.1 20210110"
	.section	.note.GNU-stack,"",%progbits
