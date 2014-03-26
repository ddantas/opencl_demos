__kernel void threshold(__global const char *a, __global char *b){
	int id = get_global_id(0);
	if ((unsigned char)a[id] >= 100)
		b[id] = (a[id]*0) + 255;
	else
		b[id] = a[id]*0;
}