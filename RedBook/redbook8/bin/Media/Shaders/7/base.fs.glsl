// base.fs.glsl
# version 430 core

uniform sampler2DShadow depth_texture; // 深度图
uniform vec3 lightPos;
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_specular_power;

layout(location = 0) out vec4 fragColor;

in vs_fs_out
{
	vec4 shadow_coord;
	vec3 world_coord;
	vec3 eye_coord;
	vec3 normal;

} info;



void main()
{
	//vec3 rgb =  vec3(0.3,0.3,0.3) + vec3(0.0f,1.0f,0.5f) * info.normal;
	//fragColor = vec4(rgb,1.0f);
    vec3 N = info.normal;
    vec3 L = normalize(lightPos - info.world_coord);
    float LdotN = dot(N, L);
    vec3 R = reflect(-L, N);

    float diffuse = max(LdotN, 0.0);
    float specular = max(pow(dot(normalize(-info.eye_coord), R), mat_specular_power), 0.0);

	//proj the shadow texture
    float f = textureProj(depth_texture, info.shadow_coord);

    fragColor = vec4(mat_ambient + f * (mat_diffuse * diffuse + mat_specular * specular), 1.0);
	

}