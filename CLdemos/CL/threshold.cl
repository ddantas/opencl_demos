__kernel void threshold (
	__global char* a ,
	__global char b ) {
		int id = get_global_id(0) ;
		if ( a[id] >= b )
			a[id] = 255 ;
		else
			a[id] = 0;
	}
