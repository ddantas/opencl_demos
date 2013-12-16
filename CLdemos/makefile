all:
	gcc -c Invert.c -I. -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86 -lOpenCL -o invert.o -std=gnu99
	gcc -I. -I.. -I../Cdemos -I/opt/AMDAPP/include CLinvert.c ../Cdemos/image.o invert.o -L/opt/AMDAPP/lib/x86 -lOpenCL -o clinvert -lm -std=gnu99
