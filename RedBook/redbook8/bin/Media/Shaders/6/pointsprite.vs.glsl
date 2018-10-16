// pointsprite.vs.glsl

#version 330

layout(location = 0)  in vec4 position;
 
uniform mat4 model_matrix;
uniform mat4 projection_matrix;


void main()
{
	gl_Position = projection_matrix * model_matrix * position;
	gl_PointSize = (1.0 - position.z /position.w) * 64;

}



 

