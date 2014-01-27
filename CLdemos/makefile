all:
	gcc -c cl_image.c ../Cdemos/image.o -I../ -I../Cdemos/ -I./ -I/opt/AMDAPP/include -I../ -L/opt/AMDAPP/lib/x86/ -lOpenCL -lm -o cl_image.o -std=gnu99
	gcc main.c cl_image.o ../Cdemos/image.o -I../ -I../Cdemos/ -I./ -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86 -lOpenCL -lm -o opencl -std=gnu99
run:
	./opencl ../images/barbara.pgm saida.pgm saida2.pgm
	display ./saida.pgm &
	display ./saida2.pgm &
clean:
	rm *.o
	rm ./opencl
	rm *.pgm
