// OIT.fs.glsl

#version 430 core

// 开启预先片元测试
layout(early_fragment_tests) in;

// head指针的2D缓存,用于保存索引信息，该iamge2d大小于屏幕大小一致，每个点对应一个位置，
// 该位置存储list_buffer中的一个位置索引,通过该索引，可以从list_buffer中获取颜色、next_inex等
layout(binding = 0, r32ui) uniform uimage2D head_pointer_image;

// 链表的1D缓存,用于保存链表的实际元素（x:next_index,y:color1,z:depth，w：color2)
layout(binding = 1, rgba32ui) uniform writeonly uimageBuffer list_buffer;

// 原子计数器，用于计算index
layout(binding = 0, offset = 0) uniform  atomic_uint  list_counter;

// output color
layout(location = 0) out vec4 color;

in vec3 frag_position;
in vec3 frag_normal;
in vec4 surface_color;

uniform vec3 light_position = vec3(40.0,20.0,100.0);

void main()
{
	uint index; // 链表的索引值，类似于指针，每绘制一次，索引值递增，保证所指的颜色数组不重复。
	uint old_head;
	uvec4 item;
	//vec4 frag_color;
	
	// 在链表缓存中分配一个索引
	// atomicCounterIncrement 会让原子计数器加1，并返回计数器之前的值
	 index = atomicCounterIncrement(list_counter);
	
	// 将片元插入到链表中,即：将一个uint类型的颜色值写入head_pointer_image中，并返回旧的颜色值
	old_head = imageAtomicExchange(head_pointer_image,ivec2(gl_FragCoord.xy),index);
	vec3 L = normalize(light_position - frag_position); // 光线方向
	vec3 V = normalize(-frag_position); // 视线方向
	vec3 N = normalize(frag_normal); // 发现方向
	vec3 H = normalize( L + V);  // 半法线方向
	
	float NdotL = dot(N, L);
	float NdotH = dot(N, H);
	vec4 modulator = vec4(surface_color.rgb * abs(NdotL), surface_color.a);
	vec4 additive_component = mix(surface_color,vec4(1.0),0.6) * vec4(pow(clamp(NdotH,0.0, 1.0),26.0)) * 0.7;
	
	// 将片元设置到缓存中
	// x:存储链表的next_pointer指针
	// y:存储color
	// z:存储depth
	item.x = old_head;
	item.y = packUnorm4x8(modulator);
	item.z = floatBitsToUint(gl_FragCoord.z);
	item.w = packUnorm4x8(additive_component);
		
	// 存储数据到缓存
	imageStore(list_buffer,int(index),item);
	
	//frag_color = modulator;
	//color = frag_color;
	
	color = modulator;
	
}
