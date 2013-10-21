opp.out : opp.c
	gcc -lOpenCL -I/opt/AMDAPP/include -I. -lm -std=gnu99 opp.c -o opp.out
