#MAKEFILE

GCC = gcc
CVFLAGS = -L/usr/local/lib/ -lopencv_highgui -lopencv_core
CVINCLUDES = -I/usr/local/include/opencv/

CLINCLUDES = -I./ -I/opt/AMDAPP/include
CLFLAGS = -L/opt/AMDAPP/lib/x86 -lOpenCL -lm -std=gnu99

all:
	echo "\nCompiling CL/CV demos\n"
	$(GCC) cl_cv_invert.c cl_image.o -o ../bin/cl_cv_invert \
	$(CLINCLUDES) $(CVINCLUDES) $(CLFLAGS) $(CVFLAGS)
