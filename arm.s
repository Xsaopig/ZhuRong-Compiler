	.arch armv7
	.file "test.c"
	.text
	.text
	.align 1
	.global main
	.arch armv7
	.syntax unified
	.thumb
	.thumb_func
	.fpu vfp
	.type main, %function
main:
	 SUB sp, sp, #84
	.size main, .-main
