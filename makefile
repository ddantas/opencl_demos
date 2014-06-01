#MAKEFILE

GCC = g++
CVFLAGS = -L/usr/local/lib/ -lopencv_highgui -lopencv_core
CVINCLUDES = -I/usr/local/include/opencv/

CLINCLUDES = -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include
CLFLAGS = -L/opt/AMDAPP/lib/x86 -lOpenCL -lm -std=gnu99

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
	gcc -c ./CLdemos/cl_image.c ./Cdemos/image.o $(CLINCLUDES) $(CLFLAGS) -o ./CLdemos/cl_image.o
	gcc ./CLdemos/main.c ./CLdemos/cl_image.o ./Cdemos/image.o $(CLINCLUDES) $(CLFLAGS) -o ./bin/opencl
	gcc ./CLdemos/clInvert.c ./CLdemos/cl_image.o ./Cdemos/image.o $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert2
	echo "\nCompiling CV demos\n"
	$(GCC) ./Cdemos/aviFile.c -o ./bin/aviFile  $(CVINCLUDES) $(CVFLAGS)
	echo "\nCompiling CL/CV demos\n"
	$(GCC) ./CLdemos/cl_cv_invert.c ./CLdemos/cl_image.o -o ./bin/cl_cv_invert $(CLINCLUDES) $(CVINCLUDES) $(CLFLAGS) $(CVFLAGS)
cvAviFile:
	$(GCC) ./Cdemos/aviFile.c -o ./bin/aviFile  $(CVINCLUDES) $(CVLDFLAGS)
clInvert2:
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o -I./Cdemos/ -I./CLdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -std=gnu99 -o ./bin/clInvert2
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
	./bin/clInvert2 ./images/barbara.pgm ./images_out/CL_SIn2_barbara.pgm
	display ./images_out/CL_SIn2_barbara.pgm
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
	./bin/clInvert2 ./images/barbara.pgm ./images_out/CL_SIn2_barbara.pgm
	display ./images_out/CL_SIn2_barbara.pgm &
run_cvAvifile:
	./bin/aviFile ./images/tree.avi
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
