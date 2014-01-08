all:
	gcc -c opencl.c -I../ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86 -lOpenCL -o opencl.o -std=gnu99
	gcc clinvert.c -I../Cdemos/ ../Cdemos/image.o -I./ ./opencl.o -I../ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86 -lOpenCL -lm -std=gnu99 -o saida
run:
	./saida ../images/barbara.pgm ./saida.pgm
	display saida.pgm &
clean:
	rm ./saida.pgm
	rm *.o
	rm saida


