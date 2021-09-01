#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other"; break;
    } std::cout << std::endl;
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

GLFWwindow* glfwSetup(bool debugMode)
{
    GLFWwindow* win;
    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    if(debugMode) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* Create a windowed mode window and its OpenGL context */
    win = glfwCreateWindow(640, 480, "Two Triangles", NULL, NULL);
    if (!win)
    {
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew Error\n";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    if (debugMode)
    {
        int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }
   
    return win;
}

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

int main(void)
{
    GLFWwindow* window = glfwSetup(1);
    if (window == NULL)return -1;

    //triangle 1
    unsigned int vao1;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);

    float vertices1[] = {
        0.1f, 0.5f, 0.0f,
        0.1f, -0.5f, 0.0f,
        0.5f, 0.1f, 0.0f
    };

    unsigned int indices1[] = {
        0, 1, 2
    };

    unsigned int vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int ebo1;
    glGenBuffers(1, &ebo1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glBindVertexArray(0);

    //triangle 2
    unsigned int vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    float vertices2[] = {
        -0.1f, 0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,
        -0.5f, 0.1f, 0.0f
    };

    unsigned int indices2[] = {
        0, 2, 1
    };

    unsigned int vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int ebo2;
    glGenBuffers(1, &ebo2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glBindVertexArray(0);

    //unbinding stuff
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //shader stuff
    std::string vertPath = "../../../source/02_twoTriangles/vertex.shader";
    std::string fragGreenPath = "../../../source/02_twoTriangles/fragmentGreen.shader";
    std::string fragRedPath = "../../../source/02_twoTriangles/fragmentRed.shader";

    unsigned int shaderRedProg = createShaderProgram(vertPath, fragRedPath);
    unsigned int shaderGreenProg = createShaderProgram(vertPath, fragGreenPath);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao1);
        glUseProgram(shaderRedProg);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vao2);
        glUseProgram(shaderGreenProg);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}