__kernel void invert(__read_only image3d_t img_input, __write_only image3d_t img_output){
	int4 coords = (int4)(get_global_id(0), get_global_id(1), get_global_id(2), 0);
	const sampler_t smp = 		CLK_NORMALIZED_COORDS_FALSE |
								CLK_ADDRESS_CLAMP 			|
								CLK_FILTER_NEAREST;
	float4 p = read_imagef(img_input, smp, (int4)(coords.x,coords.y,coords.z,0));
	p.x = 1.0f - p.x;
	write_imagef(img_output,coords,p);
}
