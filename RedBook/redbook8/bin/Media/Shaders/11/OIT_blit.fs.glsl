// OIT_blit.fs.glsl
// OpenGL programming Guid - Order Independent Transparency exmaple

#version 430 core
//#version 420 core

// The per-pixel image containing the head pointers
layout(binding = 0, r32ui) uniform uimage2D head_pointer_image; 

// Buffer containing link lists of fragments 
layout(binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

// output colors;
layout(location = 0) out vec4 color;

#define MAX_FRAGMENTS 40

// temporary array used for sorting fragments
 // framents 适用于保存,当前像素位置上有多少个片元，通过list_buffer获取在该位置的所有片元，用于后续排序处理
//uvec4 fragment_list[MAX_FRAGMENTS];
uvec4 fragments[MAX_FRAGMENTS];

// void main()
// {
	// uint current_index;
	// uint fragment_count = 0;
	
	// // 从head指针图像中获取初始的head指针
	// current_index = imageLoad(head_pointer_image,ivec2(gl_FragCoord.xy)).x;  

	// // 执行循环，直到到达链表末端,或者到达fragments[]的存储上限,
	// while(current_index != 0 && fragment_count < MAX_FRAGMENTS)
	// {
		// uvec4 fragment = imageLoad(list_buffer,int(current_index));	
		// // 将片元数据保存到数组中
		// fragment_list[fragment_count] = fragment;
		// // item.x 保存next指针的值，用它更新current
		// current_index = fragment.x;
		// fragment_count++;
	// }
	 
	// uint i,j;
    // if (fragment_count > 1)
    // {

        // for (i = 0; i < fragment_count - 1; i++)
        // {
            // for (j = i + 1; j < fragment_count; j++)
            // {
                // uvec4 fragment1 = fragment_list[i];
                // uvec4 fragment2 = fragment_list[j];

                // float depth1 = uintBitsToFloat(fragment1.z);
                // float depth2 = uintBitsToFloat(fragment2.z);

                // if (depth1 < depth2)
                // {
                    // fragment_list[i] = fragment2;
                    // fragment_list[j] = fragment1;
                // }
            // }
        // }

    // }
	
	// // 初始化颜色值
	// vec4 final_color = vec4(0.0);
	// for( i = 0; i < fragment_count; i++)
	// {
		// vec4 modulator = unpackUnorm4x8(fragment_list[i].y);
		// vec4 additive_component = unpackUnorm4x8(fragment_list[i].w);
		 // final_color = mix(final_color, modulator, modulator.a) + additive_component;
	// }
	 // color = final_color;
	
// }



int build_local_fragment_list()
{
	uint current;
	int frag_count = 0;
	
	// 从head指针图像中获取初始的head指针
	current = imageLoad(head_pointer_image,ivec2(gl_FragCoord.xy)).x;  

	// 执行循环，直到到达链表末端,或者到达fragments[]的存储上限,
	while(current != 0 && frag_count < MAX_FRAGMENTS)
	{
		uvec4 item = imageLoad(list_buffer,int(current));	
		// 将片元数据保存到数组中
		fragments[frag_count] = item;
		// item.x 保存next指针的值，用它更新current
		current = item.x;
		frag_count++;
	}
	// 返回片元数目
	return frag_count;
}
// bubble-sort 冒泡排序
void sort_fragment_list(int frag_count)
{
	if(frag_count < 1) return ; 
	int i,j;
	for(i = 0; i < frag_count-1; i++)
	{
		for( j = i + 1; j < frag_count; j++)
		{
                	uvec4 fragment1 = fragments[i];
                	uvec4 fragment2 = fragments[j];
			// 各个片元的深度需要从.z获取
			float depth_i = uintBitsToFloat(fragment1.z);
			float depth_j = uintBitsToFloat(fragment2.z);
			
			// 进行深度比较，如果失败
			if(depth_i < depth_j)
			{
				// 交换数组片元顺序
			//	uvec4 temp = fragments[i];
			//	fragments[i] = fragments[j];
			//	fragments[j] = temp;
			fragments[i] = fragment2;
			fragments[j] = fragment1;
			}
		}
	}
}

// 简单的alpha 融合函数
vec4 blend(vec4 current_color, vec4 new_color)
{
	return mix(current_color,new_color,new_color.a);
}

// 计算最终输出颜色
vec4 calculate_final_color(int frag_count)
{
	// 初始化颜色值
	vec4 final_color = vec4(0.0);
	int i;
	for( i = 0; i < frag_count; i++)
	{
		vec4 modulator = unpackUnorm4x8(fragments[i].y);
		vec4 additive_component = unpackUnorm4x8(fragments[i].w);
		// 调用融合函数
	        // final_color = blend(final_color,modulator) + additive_component;
		 final_color = mix(final_color, modulator, modulator.a) + additive_component;
	}
	 return final_color;
}
void main()
{
	int frag_count;
	frag_count = build_local_fragment_list();
	
	// sort 
	sort_fragment_list(frag_count);
	
	// outputcolor
	color = calculate_final_color(frag_count);
	
}
