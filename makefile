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
	gcc -c ./CLdemos/video.c -o ./CLdemos/video.o -I. $(CFLAGS) \
	$(CVFLAGS) $(CVINCLUDES)
	
	gcc -c ./CLdemos/cl_image.c ./CLdemos/video.o -I. -I./CLdemos/ \
	$(CINCLUDES) $(CFLAGS) \
	$(CLINCLUDES) $(CLFLAGS) -o ./CLdemos/cl_image.o
	
	gcc ./CLdemos/clInvertA.c ./CLdemos/cl_image.o ./Cdemos/image.o -I. \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvertA

	echo "\nCompiling CL/CV demos\n"
	gcc ./CLdemos/clInvert3D.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/clInvert3D
	
	gcc ./CLdemos/clInvert2D.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/clInvert2D
	
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
clInvertA:
	gcc ./CLdemos/clInvertA.c ./CLdemos/cl_image.o ./Cdemos/image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) -o ./bin/clInvertA
clInvert2D:
	gcc ./CLdemos/clInvert2D.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/clInvert2D
clInvert3D:
	gcc ./CLdemos/clInvert3D.c ./CLdemos/cl_image.o \
	$(CINCLUDES) $(CFLAGS) $(CLINCLUDES) $(CLFLAGS) \
	$(CVINCLUDES) $(CVFLAGS) -o ./bin/clInvert3D
run_cfliph:
	./bin/fliph ./input/barbara.pgm ./output/fliph.pgm
	display ./output/fliph.pgm 
run_cflipv:
	./bin/flipv ./input/barbara.pgm ./output/flipv.pgm
	display ./output/flipv.pgm
run_cinvert:
	./bin/invert ./input/barbara.pgm ./output/invert.pgm
	display ./output/invert.pgm
run_cthreshold:
	./bin/threshold ./input/barbara.pgm ./output/threshold.pgm
	display ./output/threshold.pgm 
run_cfilMed:
	./bin/filMed ./input/barbara.pgm ./output/filMed.pgm
	display ./output/filMed.pgm
run_cblur:
	./bin/blur ./input/barbara.pgm ./output/blur.pgm
	display ./output/blur.pgm
run_claplac:
	./bin/laplac ./input/barbara.pgm ./output/laplac.pgm
	display ./output/laplac.pgm
run_clInvertA:
	./bin/clInvertA ./input/barbara.pgm ./output/clInvertA.pgm
	display ./output/clInvertA.pgm
run_clInvert2D:
	./bin/clInvert2D ./input/sp.jpg
run_clInvert3D:
	./bin/clInvert3D  ./input/tree.avi
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
	./bin/clInvertA ./input/barbara.pgm ./output/clInvertA.pgm
	display ./output/clInvertA.pgm
	./bin/clInvert2D ./input/sp.jpg 3
	./bin/clInvert3D ./input/tree.avi 3

clean:
	rm ./Cdemos/image.o
	rm ./CLdemos/cl_image.o
	rm ./bin/blur*
	rm ./bin/clInvert2D*
	rm ./bin/clInvert3D*
	rm ./bin/clInvertA*
	rm ./bin/filMed*
	rm ./bin/fliph*
	rm ./bin/flipv*
	rm ./bin/invert*
	rm ./bin/laplac*
	rm ./bin/threshold*
	rm ./output/*.pgm
clclean:
	rm ./CLdemos/*.o
	rm ./bin/cl*
