	.arch armv7
	.file "testcase.sy"
	.text

	.global a
	.align 2
	.type a,%object
	.data
	.size a, 4
a:
	.word 10

	.text
	.global main
	.arch armv7
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfp
	.type main, %function
main:
	PUSH { r12, lr }
	SUB sp, sp, #8
	ADD r12, sp, #0
	MOV r14, #5
	ADD r0, r11, r14
	ADDS r12, r12, #8
	MOV sp, r12
	POP { r12, lr }
	BX lr
	.size main, .-main

