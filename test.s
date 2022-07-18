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
	.section	.rodata
	.align	2
.LC0:
	.word	1
	.word	2
	.word	2
	.word	3
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
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r7}
	sub	sp, sp, #16
	add	r7, sp, #0
	movw	r3, #:lower16:.LC0
	movt	r3, #:upper16:.LC0
	mov	r4, r7
	ldm	r3, {r0, r1, r2, r3}
	stm	r4, {r0, r1, r2, r3}
	movs	r3, #0
	mov	r0, r3
	adds	r7, r7, #16
	mov	sp, r7
	@ sp needed
	pop	{r4, r7}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Raspbian 10.2.1-6+rpi1) 10.2.1 20210110"
	.section	.note.GNU-stack,"",%progbits
