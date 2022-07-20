	.arch armv7
	.file	"test.c"

	.global y
	.type y,%object
	.data
	.size y, 8
y:
	.word 1
	.space 4


	.global a
	.type a,%object
	.data
	.size a, 16
a:
	.word 1
	.space 12

