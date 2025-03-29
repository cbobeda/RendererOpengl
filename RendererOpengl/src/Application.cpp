#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>


struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string filePath)
{
    std::ifstream stream(filePath);
    
    enum class ShaderTypes
    {
        NONE = -1,

        VERTEX = 0,

        FRAGMENT = 1
    };

    std::string line;

    std::stringstream ss[2];
    ShaderTypes type = ShaderTypes::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderTypes::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderTypes::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }

    }
    return { ss[0].str(),ss[1].str() };
}


static unsigned int compileShader(unsigned int type , const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)_malloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "failed to compile shader !" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        _freea(message);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Renderer OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
        std::cout << "error !" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr int nbInfos = 6; /*(x, y, z, r, g, b) for each vertex*/
    constexpr int nbVerticies = 4;

    constexpr int bufferSize = nbInfos * nbVerticies;
    float verticies[bufferSize] = {
        /*x, y, z, r, g, b*/
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, /*Vertex 1 (Red)*/
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, /*Vertex 2 (Green)*/
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, /*Vertex 3 (Blue)*/
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  /*Vertex 4 (Yellow)*/
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), verticies,GL_STATIC_DRAW);


    /*positions sent to the shader*/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * nbInfos, 0);

    /*colors sent to the shader*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, nbInfos * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ShaderProgramSource source = parseShader("res/shaders/Basic.shader");

    unsigned int shader = createShader(source.vertexSource, source.fragmentSource);

    glUseProgram(shader);

    float timeValue;
    int timeLocation;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        timeValue = glfwGetTime();

        timeLocation = glGetUniformLocation(shader, "u_Time");

        glUniform1f(timeLocation, timeValue);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw veticies arrays*/
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 1, 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}