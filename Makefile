all:
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -lm -I. -std=gnu99
	gcc ./Cdemos/fliph.c ./Cdemos/image.o -o ./bin/fliph.out -lm -I. -std=gnu99
	gcc ./Cdemos/flipv.c ./Cdemos/image.o -o ./bin/flipv.out -lm -I. -std=gnu99
	gcc ./Cdemos/invert.c ./Cdemos/image.o -o ./bin/invert.out -lm -I. -std=gnu99
	gcc ./Cdemos/threshold.c ./Cdemos/image.o -o ./bin/threshold.out -lm -I. -std=gnu99
	gcc ./Cdemos/filMed.c ./Cdemos/image.o -o ./bin/filMed.out -lm -I. -std=gnu99
	gcc ./Cdemos/blur.c ./Cdemos/image.o -o ./bin/blur.out -lm -I. -std=gnu99
	gcc ./Cdemos/laplac.c ./Cdemos/image.o -o ./bin/laplac.out -lm -I. -std=gnu99
image: image.c 
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -lm -I. -std=gnu99
fliph:
	gcc ./Cdemos/fliph.c ./Cdemos/image.o -o ./bin/fliph.out -lm -I. -std=gnu99
flipv:
	gcc ./Cdemos/flipv.c ./Cdemos/image.o -o ./bin/flipv.out -lm -I. -std=gnu99
invert:
	gcc ./Cdemos/invert.c ./Cdemos/image.o -o ./bin/invert.out -lm -I. -std=gnu99
threshold:
	gcc ./Cdemos/threshold.c ./Cdemos/image.o -o ./bin/threshold.out -lm -I. -std=gnu99
filMed:
	gcc ./Cdemos/filMed.c ./Cdemos/image.o -o ./bin/filMed.out -lm -I. -std=gnu99
blur:
	gcc ./Cdemos/blur.c ./Cdemos/image.o -o ./bin/blur.out -lm -I. -std=gnu99
laplac:
	gcc ./Cdemos/laplac.c ./Cdemos/image.o -o ./bin/laplac.out -lm -I. -std=gnu99
run_fliph:
	./bin/fliph.out ./images/barbara.pgm ./images_out/SFH_barbara.pgm
	display ./images_out/SFH_barbara.pgm &
run_flipv:
	./bin/flipv.out ./images/barbara.pgm ./images_out/SFV_barbara.pgm
	display ./images_out/SFV_barbara.pgm &
run_invert:
	./bin/invert.out ./images/barbara.pgm ./images_out/SIn_barbara.pgm
	display ./images_out/SIn_barbara.pgm &
run_threshold:
	./bin/threshold.out ./images/barbara.pgm ./images_out/THR_barbara.pgm
	display ./images_out/THR_barbara.pgm &
run_filMed:
	./bin/filMed.out ./images/barbara.pgm ./images_out/FMD_barbara.pgm
	display ./images_out/FMD_barbara.pgm &
run_blur:
	./bin/blur.out ./images/barbara.pgm ./images_out/BLU_barbara.pgm
	display ./images_out/BLU_barbara.pgm &
run_laplac:
	./bin/laplac.out ./images/barbara.pgm ./images_out/LAP_barbara.pgm
	display ./images_out/LAP_barbara.pgm &
clean:
	rm ./images_out/*
	rm ./bin/*.out
