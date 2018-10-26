// particlesimulator.cp.glsl
#version 430 core 

// uniform 块中包含引力器的位置和质量
layout(std140,binding=0) uniform attractor_block 
{
	vec4 attractor[64]; // xyz = position, w = mass;
};

// 每块粒子的数量为128
layout(local_size_x = 128) in;

// 使用两个缓存来包含粒子的位置和速度信息
layout(rgba32f,binding = 0) uniform imageBuffer velocity_buffer;
layout(rgba32f,binding = 1) uniform imageBuffer position_buffer;

// 时间间隔
uniform float dt;

void main()
{
	// 从缓存中读取当前位置和速度
	vec4 vel = imageLoad(velocity_buffer, int(gl_GlobalInvocationID.x));
	vec4 pos = imageLoad(position_buffer, int(gl_GlobalInvocationID.x));
	
	int i;
	
	// 使用当前速度 * 时间更新位置
	pos.xyz += vel.xyz * dt;
	// 通过w分量更新粒子的生命值
	pos.w -= 0.0001 * dt;
	
	// 对于每个引力器
	for( i = 0; i < 4; i++)
	{
		// 计算受力情况并更新速度
		vec3 dist = (attractor[i].xyz - pos.xyz);
		vel.xyz += dt * dt * attractor[i].w *
					normalize(dist)/(dot(dist,dist) + 10.0); //  
	}
	
	// 如果粒子已经到期，重置它的位置
	if(pos.w <= 0.0)
	{
		pos.xyz = -pos.xyz * 0.01;
		vel.xyz *= 0.01;
		pos.w += 1.0f;
	}
	
	// 将新的位置和速度信息重新保存到缓存中
	imageStore(position_buffer, int(gl_GlobalInvocationID.x),pos);
	imageStore(velocity_buffer, int(gl_GlobalInvocationID.x),vel);
	
}
