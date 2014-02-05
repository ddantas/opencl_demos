__kernel void Threshold(__global const char* b, __global char* c){
		int id = get_global_id(0);
		if((unsigned char)b[id] <= 100){
			c[id] = b[id]*0;
		}
		else{
			c[id] = 255 + (b[id]*0);
		}
}
