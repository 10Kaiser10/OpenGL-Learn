#include<iostream>
#include<fstream>
#include<GL/glew.h>
#include <string>
#include <shaderProc.h>

unsigned int parseShader(GLenum shaderType, std::string path)
{
    std::string line;
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.open(path);

    if (shaderFile.is_open())
    {
        while (std::getline(shaderFile, line))
        {
            shaderCode += (line + "\n");
        }
        shaderFile.close();
    }
    else
    {
        std::cout << "Could not open shader file at: " << path << std::endl;
        return 0;
    }

    unsigned int shader = glCreateShader(shaderType);
    const char* c_str = shaderCode.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    int successVertex;
    char infoLogVertex[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &successVertex);

    if (successVertex == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLogVertex);
        std::cout << "ERROR: Shader Compilation Failed at:\n" << path << std::endl << infoLogVertex << std::endl;
    }

    return shader;
}

unsigned int createShaderProgram(std::string vertexPath, std::string fragPath)
{
    unsigned int vertexShader = parseShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragShader = parseShader(GL_FRAGMENT_SHADER, fragPath);

    if (vertexShader == 0 || fragShader == 0)return 0;

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int successProgram;
    char infoLogProgram[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successProgram);

    if (successProgram == GL_FALSE)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogProgram);
        std::cout << "ERROR: Program Linking Failed:\n" << infoLogProgram << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}