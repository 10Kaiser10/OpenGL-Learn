#include <string>
#include <texture.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <iostream>

unsigned int getTexture(std::string path)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, numChannels;
	unsigned char* imgData = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Couldn't import texture: " << path << std::endl;
	}

	stbi_image_free(imgData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}