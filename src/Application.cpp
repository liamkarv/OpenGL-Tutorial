#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//----------------------------------
//Purpose: To return a shader object
//----------------------------------
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); //Creates a shader object with the given type
    const char* src = source.c_str(); //Gets shader source code 
    glShaderSource(id, 1, &src, nullptr); //Replaces shader's source code 
    glCompileShader(id); //Compiles shader code to assembly

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Gets the compile status parameter from the given shader... so we can check if it failed to compile
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id; //Returns the id of the shader, rather than the object of it
}

//----------------------------------
//Purpose: 
//----------------------------------
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); //Creates program object, where shader objects attach themselves to
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //Attaches shader objects to the program
    glAttachShader(program, vs); 
    glAttachShader(program, fs);

    glLinkProgram(program); //Links the program, AKA links the attached shaders
    glValidateProgram(program); //Validates whether the program can run, returns a string of information or empty text

    //Frees memory and undoes the effects of glCreateShader
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // Generates a buffer in the GPU's VRAM 
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // A Buffer need to be binded to be drawn, because OpenGL is a state machine
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Describes to OpenGL what the layout of the buffer is
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //Vertex Shader
    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    //Fragment Shader
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}