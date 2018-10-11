// 在着色器中使用实例计数器+TBO保存 ch03-4-Instance-2

#version 330

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;

// 使用TBO存储颜色和模型矩阵
uniform samplerBuffer color_tbo;
uniform samplerBuffer model_tbo;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out VS_OUT
{
	vec3 normal;
	vec4 color; 
} vs_out;

void main(void)
{
	// Generating the model matrix is more complex because you can't
    // store mat4 data in a TBO. Instead, we need to store each matrix
    // as four vec4 variables and assemble the matrix in the shader.
    // First, fetch the four columns of the matrix (remember, matrices are
    // stored in memory in column-primary order).
	// TBO中存储的数据是按照Vec4保存的，需要重新组装成Mat4
	vec4 col1 = texelFetch(model_tbo, gl_InstanceID * 4);
	vec4 col2 = texelFetch(model_tbo, gl_InstanceID * 4 + 1);
	vec4 col3 = texelFetch(model_tbo, gl_InstanceID * 4 + 2);
	vec4 col4 = texelFetch(model_tbo, gl_InstanceID * 4 + 3);
	
	// Now assemble the four colums into a matrix;
	mat4 model_matrix = mat4(col1, col2, col3, col4);
	
	mat4 mv = view_matrix * model_matrix;
	mat4 mvp = projection_matrix * mv;
	
	// Every Instance use a different color;
	vec4 color = texelFetch(color_tbo,gl_InstanceID);
	
	vs_out.normal = normalize(mat3(mv) * Normal);
	vs_out.color = color;
	gl_Position = mvp * Position;
	
}


// 设置TBO保存逐实例的颜色数据和模型矩阵数据