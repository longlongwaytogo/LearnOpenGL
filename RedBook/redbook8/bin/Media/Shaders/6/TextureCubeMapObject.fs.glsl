//TextureCubeMapObject.fs.glsl

#version 330 core 

in vec3 vs_fs_normal;
in vec3 vs_fs_position;

layout(location = 0) out vec4 fs_color;

uniform  samplerCube tex;
void main()
{
	// 观察空间中，点的位置，既可以理解为入射光线，相机位置(0,0,0) 和观察点，形成了入射光线
	vec3 tc = reflect(vs_fs_position,normalize(vs_fs_normal)); // 计算环境纹理
	fs_color = vec4(0.3,0.2,0.1,1.0) + vec4(0.97, 0.83,0.79,0.0) * texture(tex,tc);
}