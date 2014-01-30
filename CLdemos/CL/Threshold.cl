__kernel void Invert(__global const char* b, __global char* c){
		int id = get_global_id(0);
		if(b[id] <= 100){
			c[id] = 0;
		}
		else{
			c[id] = 255;
		}
		c[id]=0;
}
