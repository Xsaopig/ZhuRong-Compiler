	.arch armv7
	.file	"test.c"

	.global a
	.type a,%object
	.data
	.size a, 8
a:
	.word 1
	.word 2


	.global b
	.type b,%object
	.data
	.size b, 12
b:
	.word 3
	.space 8

