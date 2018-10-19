// 聚光灯 SpotLight_fs.glsl
#version 330 core

uniform vec3 ambient;
uniform vec3 light_color = vec3(1.0f,1.0f,1.0f);
uniform vec3 light_position = vec3(0,0,1);//vec3(12.0f, 32.0f, 560.0f);
uniform float shininess = 70;
uniform float strength = 1;

uniform vec3 eye_dir = vec3(0,0,1);
 
uniform float constantAttenuation = 2.0f;
uniform float linearAttenuation = 1.0f;
uniform float QuadraticAttenuation = 0.5f;

uniform vec3 coneDirection = vec3(0,0,-1);
uniform float spotCosCutoff = 30; // 聚光灯，聚光角度 
uniform float spotExponent = 2; // 聚光灯指数

vec4 object_color = vec4(0.0f,0.2f,0.4f,1.0f);

in vec3 normal;
in vec4 position;

out vec4 fragColor;

void main()
{
	//eye_dir = position.xyz;
	
	vec3 light_dir = light_position - vec3(position);
	float light_distance = length(light_dir);
	light_dir = light_dir / light_distance;
	
	float attenuation = 1.0 / (constantAttenuation +  
							   linearAttenuation * light_distance +
							   QuadraticAttenuation * light_distance * light_distance );
							   
	//  我们与聚光灯的距离
	float spotCos = dot(light_dir, -coneDirection);
	
	// 根据聚光灯的相对位置，进一步衰减
	if(spotCos < spotCosCutoff)
		attenuation = 0.0;
	else
	    attenuation *= pow(spotCos, spotExponent);
		
	vec3 halfVector = normalize(light_dir + eye_dir);
	
	float diffuse = max(0.0, dot(normal, light_dir));
	float specular = max(0.0f, dot(normal, halfVector));
	
	if(diffuse == 0.0f)
		specular = 0.0f;
	else
		specular = pow(specular,shininess) * strength;
	
	vec3 scatteredLight = ambient + diffuse * light_color  * attenuation;
	vec3 reflectedLight = light_color * specular * attenuation;
	
	vec3 rgb = min( object_color.rgb * scatteredLight + reflectedLight , vec3(1.0));
	
	fragColor = vec4(rgb,1.0f);

}