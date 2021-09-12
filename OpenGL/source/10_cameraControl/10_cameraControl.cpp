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

float deltaTime;
float lastTime;
FPSCamera* cam;

void UpdateDeltaTime()
{
    float currTime = (float) glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    cam->mousePosition(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam->scrollOffset(yoffset);
}

int main(void)
{
    GLFWwindow* window = glfwSetup(1);
    if (window == NULL)return -1;
    UpdateDeltaTime();

    glEnable(GL_DEPTH_TEST);

    //geometry stuff
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture stuff
    std::string dogePath = "../../../assets/doge.jpg";
    unsigned int dogeTex = getTexture(dogePath);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dogeTex);

    //shader stuff
    std::string vertPath = "../../../source/10_cameraControl/vertex.shader";
    std::string fragPath = "../../../source/10_cameraControl/fragment.shader";

    unsigned int shaderProg = createShaderProgram(vertPath, fragPath);
    glUseProgram(shaderProg);

    glUniform1i(glGetUniformLocation(shaderProg, "tex1"), 0);

    //camera
    cam = new FPSCamera(window, 0.5f, 0.05f, glm::vec3(0.0f, 0.0f, 3.0f));
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glm::vec3 positions[6] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
    };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        UpdateDeltaTime();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 6; i++)
        {
            float s = (float) glm::sin(glfwGetTime());
            float c = (float) glm::cos(glfwGetTime());

            cam->Update(deltaTime);
            glm::mat4 view = cam->getViewMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, false, glm::value_ptr(view));

            glm::mat4 projection = cam->getProjectionMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, false, glm::value_ptr(projection));

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, (i-2.5f) * (float)glfwGetTime() * glm::radians(-40.0f), glm::vec3(0.7f, 0.4f, 0.2f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, false, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}