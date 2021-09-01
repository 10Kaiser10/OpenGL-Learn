#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderCode =  "#version 460 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main(){\n"
                                "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}\n";

const char* fragShaderCode = "#version 460 core\n"
                             "out vec4 FragColor;\n"
                             "void main(){\n"
                             "FragColor = vec4(1.0, 0.5f, 0.2f, 1.0f);}\n";

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew Error\n";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    //creating vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //creating vertex buffer
    float vertices[] = {
         0.5, 0.5, 0,
        -0.5, 0.5, 0,
        -0.5,-0.5, 0,
         0.5,-0.5, 0
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //creating element buffer
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int successVertex;
    char infoLogVertex[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);

    if (successVertex == GL_FALSE)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
        std::cout << "ERROR: Vertex Shader Compilation Failed:\n" << infoLogVertex << std::endl;
    }

    //fragment shader
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderCode, NULL);
    glCompileShader(fragShader);

    int successFrag;
    char infoLogFrag[512];
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successFrag);

    if (successFrag == GL_FALSE)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLogFrag);
        std::cout << "ERROR: Fragment Shader Compilation Failed:\n" << infoLogFrag << std::endl;
    }

    //shader program
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

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

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