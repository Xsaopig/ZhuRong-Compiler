	.arch armv7
	.file	"test.c"

	.global d
	.type d,%object
	.data
	.size d, 48
d:
	.word 1
	.word 2
	.word 3
	.word 5
	.word 7
	.word 8
	.space 24


	.global e
	.type e,%object
	.data
	.size e, 32
e:
	.word 2
	.word 1
	.word 3
	.word 4
	.word 5
	.word 6
	.word 7
	.word 8

