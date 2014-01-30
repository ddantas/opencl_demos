__kernel void Invert(__global const char* b, __global char* c){
		int id = get_global_id(0);
		c[id] = 255 - b[id];
}
