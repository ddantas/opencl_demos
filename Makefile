all:
	gcc -c -I. -lm -std=gnu99 -o src/c/image.o src/c/image.c
	gcc -I. -lm -std=gnu99 -o bin/c/show src/c/image.o src/c/show.c
	gcc -I. -lm -std=gnu99 -o bin/c/invert src/c/image.o src/c/invert.c
	gcc -I. -lm -std=gnu99 -o bin/c/threshold src/c/image.o src/c/threshold.c
	gcc -I. -lm -std=gnu99 -o bin/c/media src/c/image.o src/c/media.c
	gcc -I. -lm -std=gnu99 -o bin/c/flipv src/c/image.o src/c/flipv.c
	gcc -I. -lm -std=gnu99 -o bin/c/fliph src/c/image.o src/c/fliph.c
	gcc -I. -lm -std=gnu99 -o bin/c/blur src/c/image.o src/c/blur.c
	gcc -I. -lm -std=gnu99 -o bin/c/laplacian src/c/image.o src/c/laplacian.c
	gcc -I. -lm -std=gnu99 -o bin/c/threshold src/c/image.o src/c/threshold.c
	
	gcc -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ --std=gnu99 -o bin/opencl/arraysum src/opencl/arraysum.c -lOpenCL
	gcc -c -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -I./src/c/ -I./src/opencl/ -I./ -lm -std=gnu99 ./src/opencl/cl_image.c ./src/c/image.o -o ./src/opencl/cl_image.o -lOpenCL
	gcc -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -I./src/c/ -I./src/opencl/ -I./ -lm -std=gnu99 -o ./bin/opencl/invertcl ./src/c/image.o ./src/opencl/cl_image.o ./src/opencl/cl_invert.c -lOpenCL
	gcc -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86/ -I./src/c/ -I./src/opencl/ -I./ -lm -std=gnu99 -o ./bin/opencl/thresholdcl ./src/c/image.o ./src/opencl/cl_image.o ./src/opencl/cl_threshold.c -lOpenCL
	

show:
	./bin/c/show ./images/P5.pgm ./output/P5.pgm
	display ./output/P5.pgm &

invert:
	./bin/c/invert ./images/P5.pgm ./output/inv_P5.pgm
	display ./output/inv_P5.pgm &
	
threshold:
	./bin/c/threshold ./images/P5.pgm ./output/thd_P5.pgm
	display ./output/thd_P5.pgm &
	
media:
	./bin/c/media ./images/P5.pgm ./output/med_P5.pgm
	display ./output/med_P5.pgm &
	
flipv:
	./bin/c/flipv ./images/P5.pgm ./output/fv_P5.pgm
	display ./output/fv_P5.pgm &
	
fliph:
	./bin/c/fliph ./images/P5.pgm ./output/fh_P5.pgm
	display ./output/fh_P5.pgm &
	
blur:
	./bin/c/blur ./images/P5.pgm ./output/blr_P5.pgm
	display ./output/blr_P5.pgm &
	
laplacian:
	./bin/c/laplacian ./images/P5.pgm ./output/lap_P5.pgm
	display ./output/lap_P5.pgm &
	
arraysum:
	./bin/opencl/arraysum
	
invertcl:
	./bin/opencl/invertcl ./images/P5.pgm ./output/cl_inv_P5.pgm
	display ./output/cl_inv_P5.pgm
	
thresholdcl:
	./bin/opencl/thresholdcl ./images/P5.pgm ./output/cl_thd_P5.pgm
	display ./output/cl_thd_P5.pgm

clean: 
	rm ./bin/c/* ./bin/opencl/* ./output/*