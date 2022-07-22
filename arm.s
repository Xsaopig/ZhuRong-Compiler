	.arch armv7
	.file	"test.c"

	.global a
	.type a,%object
	.data
	.size a, 32
a:
	.word 1
	.word 2
	.word 3
	.word 4
	.word 0
	.word 0
	.word 7
	.space 4


	.global c
	.type c,%object
	.data
	.size c, 32
c:
	.word 1
	.word 2
	.word 3
	.word 4
	.word 5
	.word 6
	.word 7
	.word 8


	.global d
	.type d,%object
	.data
	.size d, 32
d:
	.word 1
	.word 2
	.word 3
	.word 0
	.word 5
	.word 0
	.word 7
	.word 8


	.global e
	.type e,%object
	.data
	.size e, 32
e:
	.word 0
	.word 6
	.word 3
	.word 4
	.word 5
	.word 6
	.word 7
	.word 8

