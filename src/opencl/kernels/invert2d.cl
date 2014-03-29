__kernel void invert(__read_only  image2d_t img_input, __write_only  image2d_t img_output){
	int2 coords = (int2)(get_global_id(0), get_global_id(1));
	const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
	float4 p = read_imagef(img_input, smp, coords);
	p.w = 1.0 - p.w;
	write_imagef(img_output, coords, p);
}