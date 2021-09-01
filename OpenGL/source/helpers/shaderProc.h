#pragma once
#include<GL/glew.h>
#include<string>

unsigned int parseShader(GLenum shaderType, std::string path);
unsigned int createShaderProgram(std::string vertexPath, std::string fragPath);