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
	.global	not
	.section	.rodata
	.align	2
	.type	not, %object
	.size	not, 16
not:
	.word	0
	.word	1073741824
	.word	1077936128
	.space	4
	.global	a
	.data
	.align	2
	.type	a, %object
	.size	a, 4
a:
	.word	2
	.global	b
	.section	.rodata
	.align	2
	.type	b, %object
	.size	b, 4
b:
	.word	5
	.global	c
	.data
	.align	2
	.type	c, %object
	.size	c, 4
c:
	.word	1073741824
	.global	d
	.section	.rodata
	.align	2
	.type	d, %object
	.size	d, 4
d:
	.word	1069547520
	.global	e
	.data
	.align	2
	.type	e, %object
	.size	e, 12
e:
	.word	1
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	add	r7, sp, #0
	movs	r3, #0
	mov	r0, r3
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Raspbian 10.2.1-6+rpi1) 10.2.1 20210110"
	.section	.note.GNU-stack,"",%progbits
