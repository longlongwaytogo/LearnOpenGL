 // DoubleWrite_fs.glsl
 #version 430 core
 
 // buffer containing a palette of colors to be used to mark primitives by ID
 layout(binding = 0,rgba32f) uniform imageBuffer colors;
 
 // The buffer that we will write to 
 layout(binding = 1, rgba32f) uniform image2D output_buffer;
 
 
 out vec4 color;
 
 
in vec4 surface_color;
in vec3 frag_position;
in vec3 frag_normal;

 void main()
 {
 /*
 gl_FragCoord是个vec4，四个分量分别对应x, y, z和1/w。其中，x和y是当前片元的窗口相对坐标，不过它们不是整数，小数部分恒为0.5。x - 0.5和y - 0.5分别位于[0, windowWidth - 1]和[0, windowHeight - 1]内。windowWidth和windowHeight都以像素为单位，亦即用glViewPort指定的宽高。w即为乘过了投影矩阵之后点坐标的w，用于perspective divide的那个值。gl_FragCoord.z / gl_FragCoord.w可以得到当前片元和camera之间的距离
--------------------- 
作者：cremesino 
来源：CSDN 
原文：https://blog.csdn.net/cremesino/article/details/4132088 
版权声明：本文为博主原创文章，转载请附上博文链接！
 */
 // gl_FragCoord 坐标，是当前顶点绘制到窗口的位置(模型可能位于窗口中央)，vs->ps gl_fragCoord.xy 肯定不会是从0开始，所以可以 减掉ivec2(200,0)
	// Load a color from the palette buffer base on primitive id % 256
	
	vec4 col = imageLoad(colors,gl_PrimitiveID &255); // 读取1D 纹理数据
	
	// Store the resulting fragment at two locations First at the fragments
	// window space coordnite shift left 
	imageStore(output_buffer, ivec2(gl_FragCoord.xy) - ivec2(200,0),col);
	
	imageStore(output_buffer, ivec2(gl_FragCoord.xy) + ivec2(200,0),col);
	
 }