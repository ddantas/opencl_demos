all:
	gcc -c image.c -o image.o -lm -I.. -std=gnu99
	gcc fliph.c image.o -o ../bin/fliph.out -lm -I.. -std=gnu99
	gcc flipv.c image.o -o ../bin/flipv.out -lm -I.. -std=gnu99
	gcc invert.c image.o -o ../bin/invert.out -lm -I.. -std=gnu99
image: image.c 
	gcc -c image.c -o image.o -lm -I.. -std=gnu99
fliph:
	gcc fliph.c image.o -o ../bin/fliph.out -lm -I.. -std=gnu99
flipv:
	gcc flipv.c image.o -o ../bin/flipv.out -lm -I.. -std=gnu99
invert:
	gcc invert.c image.o -o ../bin/invert.out -lm -I.. -std=gnu99
run flip h:
	../bin/fliph.out ../images/barbara.pgm ../images_out/SFH_barbara.pgm
	display ../images_out/SFH_barbara.pgm
run flip v:
	../bin/flipv.out ../images/barbara.pgm ../images_out/SFV_barbara.pgm
	display ../images_out/SFV_barbara.pgm
run invert c:
	../bin/invert.out ../images/barbara.pgm ../images_out/SIn_barbara.pgm
	display ../images_out/SIn_barbara.pgm
