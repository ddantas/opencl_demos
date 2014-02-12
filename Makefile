all:
	echo "Compiling C demos\n"
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -lm -I. -std=gnu99
	gcc ./Cdemos/fliph.c ./Cdemos/image.o -o ./bin/fliph.out -lm -I. -std=gnu99
	gcc ./Cdemos/flipv.c ./Cdemos/image.o -o ./bin/flipv.out -lm -I. -std=gnu99
	gcc ./Cdemos/invert.c ./Cdemos/image.o -o ./bin/invert.out -lm -I. -std=gnu99
	gcc ./Cdemos/threshold.c ./Cdemos/image.o -o ./bin/threshold.out -lm -I. -std=gnu99
	gcc ./Cdemos/filMed.c ./Cdemos/image.o -o ./bin/filMed.out -lm -I. -std=gnu99
	gcc ./Cdemos/blur.c ./Cdemos/image.o -o ./bin/blur.out -lm -I. -std=gnu99
	gcc ./Cdemos/laplac.c ./Cdemos/image.o -o ./bin/laplac.out -lm -I. -std=gnu99
	echo "Compiling CL demos\n"
	gcc -c ./CLdemos/cl_image.c ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./CLdemos/cl_image.o
	gcc ./CLdemos/main.c ./CLdemos/cl_image.o ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./bin/opencl
	gcc ./CLdemos/clInvert.c ./CLdemos/cl_image.o ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./bin/clInvert
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./CLdemos/clInvert2
clInvert2:
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./CLdemos/clInvert2
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
run_all:
	./bin/fliph.out ./images/barbara.pgm ./images_out/SFH_barbara.pgm
	display ./images_out/SFH_barbara.pgm 
	./bin/flipv.out ./images/barbara.pgm ./images_out/SFV_barbara.pgm
	display ./images_out/SFV_barbara.pgm 
	./bin/invert.out ./images/barbara.pgm ./images_out/SIn_barbara.pgm
	display ./images_out/SIn_barbara.pgm 
	./bin/threshold.out ./images/barbara.pgm ./images_out/THR_barbara.pgm
	display ./images_out/THR_barbara.pgm 
	./bin/filMed.out ./images/barbara.pgm ./images_out/FMD_barbara.pgm
	display ./images_out/FMD_barbara.pgm 
	./bin/blur.out ./images/barbara.pgm ./images_out/BLU_barbara.pgm
	display ./images_out/BLU_barbara.pgm 
	./bin/laplac.out ./images/barbara.pgm ./images_out/LAP_barbara.pgm
	display ./images_out/LAP_barbara.pgm 
	./bin/clInvert ./images/barbara.pgm ./images_out/CL_SIn_barbara.pgm
	display ./images_out/CL_SIn_barbara.pgm 
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
run_clInvert:
	./bin/clInvert ./images/barbara.pgm ./images_out/CL_SIn_barbara.pgm
	display ./images_out/CL_SIn_barbara.pgm &
run_clInvert2:
	./CLdemos/clInvert2 ./images/barbara.pgm ./CLdemos/saida.pgm
	display ./CLdemos/saida.pgm
run_opencl:
	./bin/opencl ./images/barbara.pgm ./images_out/saida.pgm ./images_out/saida2.pgm
	display ./images_out/saida.pgm &
	display ./images_out/saida2.pgm &
clean_all:
	rm ./CLdemos/*.o
	rm ./bin/*
	rm ./images_out/*
clean_opencl:
	rm ./CLdemos/*.o
	rm ./bin/opencl
clean:
	rm ./images_out/*
	rm ./bin/*.out
