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
	.data
	.align	2
	.type	a, %object
	.size	a, 4
a:
	.word	3
	.global	b
	.align	2
	.type	b, %object
	.size	b, 4
b:
	.word	1084227584
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
	.fnstart
.LFB0:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #5
	str	r3, [r7, #4]
	ldr	r3, [r7, #4]
	cmp	r3, #5
	ble	.L2
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	str	r3, [r7, #4]
	b	.L3
.L2:
	movw	r3, #:lower16:b
	movt	r3, #:upper16:b
	vldr.32	s15, [r3]
	vldr.32	s14, .L5
	vadd.f32	s15, s15, s14
	movw	r3, #:lower16:b
	movt	r3, #:upper16:b
	vstr.32	s15, [r3]
.L3:
	ldr	r3, [r7, #4]
	vmov	s15, r3	@ int
	vcvt.f32.s32	s14, s15
	movw	r3, #:lower16:b
	movt	r3, #:upper16:b
	vldr.32	s15, [r3]
	vadd.f32	s15, s14, s15
	vcvt.s32.f32	s15, s15
	vmov	r3, s15	@ int
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
.L6:
	.align	2
.L5:
	.word	1065353216
	.cantunwind
	.fnend
	.size	main, .-main
	.ident	"GCC: (Raspbian 10.2.1-6+rpi1) 10.2.1 20210110"
	.section	.note.GNU-stack,"",%progbits
