#simple makefile
image.out : image.c
	gcc image.c -o bin/image.out -lm -I . -std=gnu99
