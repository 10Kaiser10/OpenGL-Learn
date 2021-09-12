#version 460 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex1;

void main()
{
	FragColor = texture(tex1, texCoord);
}