#version 460 core

in vec3 col;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), col.x);
}