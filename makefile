#MAKEFILE

GCC = g++

CFLAGS  = -lm -std=gnu99
CINCLUDES = -I./Cdemos/

CVFLAGS = -L/usr/local/lib/ -lopencv_highgui -lopencv_core \
		  -lopencv_imgproc

CVINCLUDES = -I/usr/local/include/opencv/

CLINCLUDES = -I/opt/AMDAPP/include
CLFLAGS = -L/opt/AMDAPP/lib/x86 -lOpenCL

all:
	echo "Compiling C demos\n"
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -I. $(CFLAGS)
	gcc ./Cdemos/fliph.c       ./Cdemos/image.o -o ./bin/fliph     -I. $(CFLAGS)
	gcc ./Cdemos/flipv.c       ./Cdemos/image.o -o ./bin/flipv     -I. $(CFLAGS)
	gcc ./Cdemos/invert.c      ./Cdemos/image.o -o ./bin/invert    -I. $(CFLAGS)
	gcc ./Cdemos/threshold.c   ./Cdemos/image.o -o ./bin/threshold -I. $(CFLAGS)
	gcc ./Cdemos/filMed.c      ./Cdemos/image.o -o ./bin/filMed    -I. $(CFLAGS)
	gcc ./Cdemos/blur.c        ./Cdemos/image.o -o ./bin/blur      -I. $(CFLAGS)
	gcc ./Cdemos/laplac.c      ./Cdemos/image.o -o ./bin/laplac    -I. $(CFLAGS)
	echo "Compiling CL demos\n"
	gcc -c ./CLdemos/cl_image.c ./Cdemos/image.o $(CINCLUDES) $(CFLAGS) \
	$(CLINCLUDES) $(CLFLAGS) -o ./CLdemos/cl_image.o
	
	gcc ./CLdemos/main.c ./CLdemos/cl_image.o ./Cdemos/image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/opencl
	
	gcc ./CLdemos/clInvert.c ./CLdemos/cl_image.o ./Cdemos/image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert
	
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o  \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert2
	
	echo "\nCompiling CV demos\n"
	g++ ./Cdemos/aviFile.c -o ./bin/aviFile  $(CVINCLUDES) $(CVFLAGS)
	
	echo "\nCompiling CL/CV demos\n"
	gcc ./CLdemos/cl_cv_invert.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/cl_cvinvert
	
	gcc ./CLdemos/cl_cv_invert2.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/cl_cvinvert2
	
cImage:
	gcc -c ./Cdemos/image.c -o ./Cdemos/image.o -I. $(CFLAGS)
cFlipH:
	gcc ./Cdemos/fliph.c       ./Cdemos/image.o -o ./bin/fliph     -I. $(CFLAGS)
cFlipV:
	gcc ./Cdemos/flipv.c       ./Cdemos/image.o -o ./bin/flipv     -I. $(CFLAGS)
cInvert:
	gcc ./Cdemos/invert.c      ./Cdemos/image.o -o ./bin/invert    -I. $(CFLAGS)
cThreshold:
	gcc ./Cdemos/threshold.c   ./Cdemos/image.o -o ./bin/threshold -I. $(CFLAGS)
cFilMed:
	gcc ./Cdemos/filMed.c      ./Cdemos/image.o -o ./bin/filMed    -I. $(CFLAGS)
cBlur:
	gcc ./Cdemos/blur.c        ./Cdemos/image.o -o ./bin/blur      -I. $(CFLAGS)
cLaplac:
	gcc ./Cdemos/laplac.c      ./Cdemos/image.o -o ./bin/laplac    -I. $(CFLAGS)
clImage:
	gcc -c ./CLdemos/cl_image.c ./Cdemos/image.o $(CINCLUDES) $(CFLAGS) \
	$(CLINCLUDES) $(CLFLAGS) -o ./CLdemos/cl_image.o
clMain:
	gcc ./CLdemos/main.c ./CLdemos/cl_image.o ./Cdemos/image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/opencl
clInvert:
	gcc ./CLdemos/clInvert.c ./CLdemos/cl_image.o ./Cdemos/image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert
clInvert2:
	gcc ./CLdemos/clInvert2.c ./CLdemos/cl_image.o ./Cdemos/image.o  \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvert2
cvAvi3D:
	g++ ./Cdemos/aviFile.c -o ./bin/aviFile  $(CVINCLUDES) $(CVFLAGS)
run:
	./bin/fliph ./input/barbara.pgm ./output/fliph.pgm
	display ./output/fliph.pgm 
	./bin/flipv ./input/barbara.pgm ./output/flipv.pgm
	display ./output/flipv.pgm 
	./bin/invert ./input/barbara.pgm ./output/invert.pgm
	display ./output/invert.pgm 
	./bin/threshold ./input/barbara.pgm ./output/threshold.pgm
	display ./output/threshold.pgm 
	./bin/filMed ./input/barbara.pgm ./output/filMed.pgm
	display ./output/filMed.pgm 
	./bin/blur ./input/barbara.pgm ./output/blur.pgm
	display ./output/blur.pgm 
	./bin/laplac ./input/barbara.pgm ./output/laplac.pgm
	display ./output/laplac.pgm 
	./bin/clInvert ./input/barbara.pgm ./output/clInvert.pgm
	display ./output/clInvert.pgm
	./bin/clInvert2 ./input/barbara.pgm ./output/clInvert2.pgm
	display ./output/clInvert2.pgm

clean:
	rm ./Cdemos/*.o
	rm ./CLdemos/*.o
	rm ./bin/*
	rm ./output/*
clclean:
	rm ./CLdemos/*.o
	rm ./bin/cl*
