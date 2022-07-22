	.arch armv7
	.file "test.c"
	.text

	.global a
	.align 2
	.type a,%object
	.data
	.size a, 4
a:
	.word 1


	.global b
	.align 2
	.type b,%object
	.data
	.size b, 12
b:
	.word 1
	.word 2
	.word 3

