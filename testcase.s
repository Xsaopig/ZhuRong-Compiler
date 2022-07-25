	.arch armv7
	.file "testcase.sy"
	.text

	.global b
	.align 2
	.type b,%object
	.data
	.size b, 4
b:
	.word 5

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
	MOV r11, #5
	MOVW r14, #:lower16:b
	MOVT r14, #:upper16:b
	LDR r14, [r14, #0]
	ADD r0, r11, r14
	ADDS r12, r12, #8
	MOV sp, r12
	POP { r12, lr }
	BX lr
	.size main, .-main

