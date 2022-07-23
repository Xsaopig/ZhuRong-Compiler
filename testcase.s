	.arch armv7
	.file "testcase.sy"
	.text
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
	MOV r0, #3
	ADDS r12, r12, #8
	MOV sp, r12
	POP { r12, lr }
	BX lr
	.size main, .-main

