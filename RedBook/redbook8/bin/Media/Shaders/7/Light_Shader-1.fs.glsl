// ch07-1 light_shader-1.fs.glsl
// 方向光模拟 (如太阳光)
// 在片元着色中计算方向光照

#version 330 core

uniform vec3 ambient =  vec3(0.1f, 0.2f, 0.5f);
uniform vec3 lightColor = vec3(1.0f,1.0f,1.0f);
uniform vec3 lightDirection = vec3(0,0.1,1);  // 方向光的方向
//uniform vec3 halfVector;     		// 光泽度的表面方向
uniform float shininess = 77.0f;    // 高光尖锐程度指数
uniform float strength = 2;     		//  用于调整光泽度的强度

in vec4 vs_fs_color;
in vec3 normal;
in vec4 position; 

out vec4 fragColor;

void main()
{
	float diffuse = max(0.0,dot(normal,lightDirection)); // 散射光

	vec3 halfVector = normalize(lightDirection + normalize(position).xyz );
	
	float specular = max(0.0,dot(normal,halfVector));    // 镜面光
	
	// 远离光源
	if(diffuse == 0.0)
		specular = 0.0;
	else
	specular = pow(specular,shininess); // 镜面高光 
	
	vec3 scatteredLight = ambient + lightColor * diffuse;
	vec3 reflectedLight = lightColor * specular * strength;
	
	// 不要将表面颜色与反射光进行调制，只需要和散射光相加
	vec3 rgb = min(vs_fs_color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	
	fragColor = vec4(rgb,1);
	//fragColor = vec4(1,0,0,1);
}