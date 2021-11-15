#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
out vec3 fragPos;
out vec3 normalDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	fragPos = (model * vec4(aPos, 1.0)).xyz;
	normalDir = normalize(normalMat * normal);
}