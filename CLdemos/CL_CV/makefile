#MAKEFILE

GCC = gcc
CVFLAGS = -L/usr/local/lib/ -lopencv_highgui -lopencv_core -lm -lopencv_imgproc
CVINCLUDES = -I/usr/local/include/opencv/

CLINCLUDES = -I./ -I/opt/AMDAPP/include
CLFLAGS = -L/opt/AMDAPP/lib/x86 -lOpenCL -std=gnu99

all:
	$(GCC) -c cl_image.c -o cl_image.o $(CLINCLUDES) $(CVINCLUDES) $(CLFLAGS) $(CVFLAGS)
	$(GCC) clInvert2.c cl_image.o -o img2D $(CLINCLUDES) $(CVINCLUDES) $(CLFLAGS) $(CVFLAGS)
	$(GCC) cl_cv_invert.c cl_image.o -o avi3D $(CLINCLUDES) $(CVINCLUDES) $(CLFLAGS) $(CVFLAGS)
clean:
	rm cl_image.o
	rm avi3D
	rm img2D
