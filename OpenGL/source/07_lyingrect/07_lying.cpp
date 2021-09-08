#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <setup.h>
#include <shaderProc.h>
#include <texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    std::string vertPath = "../../../source/07_lyingrect/vertex.shader";
    std::string fragPath = "../../../source/07_lyingrect/fragment.shader";

    unsigned int shaderProg = createShaderProgram(vertPath, fragPath);
    glUseProgram(shaderProg);

    glUniform1i(glGetUniformLocation(shaderProg, "tex1"), 0);
    glUniform1i(glGetUniformLocation(shaderProg, "tex2"), 1);

    //transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 640.0f / 640.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, false, glm::value_ptr(projection));

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