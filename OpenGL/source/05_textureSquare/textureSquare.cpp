#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <setup.h>
#include <shaderProc.h>
#include <texture.h>

int main(void)
{
    GLFWwindow* window = glfwSetup(1);
    if (window == NULL)return -1;

    //geometry stuff
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //texture stuff
    std::string doomPath = "../../../assets/MFDOOM.jpeg";
    std::string gradPath = "../../../assets/graduation.jpeg";
    unsigned int doomTex = getTexture(doomPath);
    unsigned int gradTex = getTexture(gradPath);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, doomTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gradTex);

    //shader stuff
    std::string vertPath = "../../../source/05_textureSquare/vertex.shader";
    std::string fragPath = "../../../source/05_textureSquare/fragment.shader";

    unsigned int shaderProg = createShaderProgram(vertPath, fragPath);
    glUseProgram(shaderProg);

    glUniform1i(glGetUniformLocation(shaderProg, "tex1"), 0);
    glUniform1i(glGetUniformLocation(shaderProg, "tex2"), 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}