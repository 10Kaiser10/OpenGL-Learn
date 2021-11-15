#version 460 core 

out vec4 FragColor;
in vec3 fragPos;
in vec3 normalDir;

uniform vec4 lightColor;
uniform vec4 objectColor;
uniform vec3 lightPos;
uniform vec3 eyePos;

void main()
{
	//ambient
	float ambientStr = 0.1f;
	vec4 ambient = ambientStr * lightColor;

	//diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseStr = max(0.0f, dot(lightDir, normalDir));
	vec4 diffuse = diffuseStr * lightColor;

	//specular
	float specularStr = 0.5f;
	vec3 viewDir = normalize(eyePos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, normalDir);
	vec4 specular = specularStr * lightColor * pow(max(0.0, dot(reflectionDir, viewDir)), 32);

	FragColor = (ambient + diffuse + specular) * objectColor;
}