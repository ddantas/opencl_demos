all:
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -lm -I. -std=gnu99
	gcc ./Cdemos/fliph.c ./Cdemos/image.o -o ./bin/fliph.out -lm -I. -std=gnu99
	gcc ./Cdemos/flipv.c ./Cdemos/image.o -o ./bin/flipv.out -lm -I. -std=gnu99
	gcc ./Cdemos/invert.c ./Cdemos/image.o -o ./bin/invert.out -lm -I. -std=gnu99
image: image.c 
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -lm -I. -std=gnu99
fliph:
	gcc ./Cdemos/fliph.c ./Cdemos/image.o -o ./bin/fliph.out -lm -I. -std=gnu99
flipv:
	gcc ./Cdemos/flipv.c ./Cdemos/image.o -o ./bin/flipv.out -lm -I. -std=gnu99
invert:
	gcc ./Cdemos/invert.c ./Cdemos/image.o -o ./bin/invert.out -lm -I. -std=gnu99
runfliph:
	./bin/fliph.out ./images/barbara.pgm ./images_out/SFH_barbara.pgm
	display ./images_out/SFH_barbara.pgm
runflipv:
	./bin/flipv.out ./images/barbara.pgm ./images_out/SFV_barbara.pgm
	display ./images_out/SFV_barbara.pgm
runinvert:
	./bin/invert.out ./images/barbara.pgm ./images_out/SIn_barbara.pgm
	display ./images_out/SIn_barbara.pgm
