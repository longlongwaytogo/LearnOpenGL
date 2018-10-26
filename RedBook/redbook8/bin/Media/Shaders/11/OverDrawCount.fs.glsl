 // overDraw_count_fs.glsl
 #version 430 core

 // The overdraw count buffer 
 layout(binding = 0, r32ui) uniform uimage2D output_buffer;
 
 
 out vec4 color;
 
 
in vec4 surface_color;
in vec3 frag_position;
in vec3 frag_normal;

 void main()
 {
 // Increment the overdraw counter
    //*
    imageAtomicAdd(output_buffer, ivec2(gl_FragCoord.xy), 1);
    /**/

    /*
    // Read the current overdraw counter
    uint count = imageLoad(output_buffer, ivec2(gl_FragCoord.xy)).x;
    // Add one
    count = count + 1;
    // Write it back to the image
    imageStore(output_buffer, ivec2(gl_FragCoord.xy), uvec4(count));
    /**/
	
 }