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
#include <FPSCam.h>

//time keeping variables
float deltaTime;
float lastTime;

//camera instance
FPSCamera* cam;

void UpdateDeltaTime()
{
    float currTime = (float)glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    //cam->mousePosition(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //cam->scrollOffset(yoffset);
}

int main(void)
{
    GLFWwindow* window = glfwSetup(1);
    if (window == NULL)return -1;
    UpdateDeltaTime();

    glEnable(GL_DEPTH_TEST);

    //vertex data
    float sourceVertices[] = {
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,
    };

    float objectVertices[] = {
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,
    };

    int indices[] = {
        0, 2, 3,
        0, 3, 1,
        0, 4, 2,
        2, 4, 6,
        0, 1, 5,
        0, 5, 4,
        1, 3, 5,
        3, 7, 5,
        5, 7, 4,
        4, 7, 6,
        2, 6, 7,
        2, 7, 3,
    };

    //source light vao
    unsigned int sourceVao;
    glGenVertexArrays(1, &sourceVao);
    glBindVertexArray(sourceVao);

    unsigned int sourceVbo;
    glGenBuffers(1, &sourceVbo);
    glBindBuffer(GL_ARRAY_BUFFER, sourceVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sourceVertices), sourceVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int sourceEbo;
    glGenBuffers(1, &sourceEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sourceEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //unbinding buffers and vao
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //object vao
    unsigned int objectVao;
    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    unsigned int objectVbo;
    glGenBuffers(1, &objectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, objectVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objectVertices), objectVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int objectEbo;
    glGenBuffers(1, &objectEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //unbinding buffers and vao
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //shaders
    std::string sourceVertPath = "../../../source/11_lightingSetup/sourceVertex.shader";
    std::string sourceFragPath = "../../../source/11_lightingSetup/sourceFragment.shader";
    std::string objectVertPath = "../../../source/11_lightingSetup/objectVertex.shader";
    std::string objectFragPath = "../../../source/11_lightingSetup/objectFragment.shader";

    unsigned int sourceShaderProg = createShaderProgram(sourceVertPath, sourceFragPath);
    unsigned int objectShaderProg = createShaderProgram(objectVertPath, objectFragPath);

    glm::vec4 lightColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    glm::vec4 objectColor = glm::vec4(1.0, 0.3, 0.6, 1.0);
    glUseProgram(sourceShaderProg);
    glUniform4fv(glGetUniformLocation(sourceShaderProg, "lightColor"), 1, glm::value_ptr(lightColor));
    glUseProgram(objectShaderProg);
    glUniform4fv(glGetUniformLocation(objectShaderProg, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform4fv(glGetUniformLocation(objectShaderProg, "objectColor"), 1, glm::value_ptr(objectColor));

    //camera
    cam = new FPSCamera(window, 1.0f, 0.05f, glm::vec3(0.0f, 3.0f, -7.0f), glm::normalize(glm::vec3(0.0f, -0.3f, 1.0f)));
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        UpdateDeltaTime();

        //camera dependent matrices
        cam->Update(deltaTime);
        glm::mat4 view = cam->getViewMatrix();
        glm::mat4 projection = cam->getProjectionMatrix();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);


        //source model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0, 3.0, 3.0));
        model = glm::scale(model, glm::vec3(0.2f));

        glBindVertexArray(sourceVao);
        glUseProgram(sourceShaderProg);
        glUniformMatrix4fv(glGetUniformLocation(sourceShaderProg, "view"), 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sourceShaderProg, "projection"), 1, false, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(sourceShaderProg, "model"), 1, false, glm::value_ptr(model));

        glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, 0);

        //object model matrix
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));

        glBindVertexArray(objectVao);
        glUseProgram(objectShaderProg);
        glUniformMatrix4fv(glGetUniformLocation(objectShaderProg, "view"), 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(objectShaderProg, "projection"), 1, false, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(objectShaderProg, "model"), 1, false, glm::value_ptr(model));

        glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}