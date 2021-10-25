//defines
#define GLEW_STATIC
//includes 
#include <GLEW\include\GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static unsigned int compile_shader(unsigned int type , const std::string &source) 
{

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id , 1 , &src , nullptr);
    glCompileShader(id);

    //error handling
    int result;
    glGetShaderiv(id , GL_COMPILE_STATUS , &result);

    if (result == GL_FALSE) 
    {

        int length;
        glGetShaderiv(id , GL_INFO_LOG_LENGTH , &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id , length , &length , message);

        std::cout << "Failed to compile shader" << message << std::endl;

        glDeleteShader(id);

        return 0;
    
    }

    return id;

}

static unsigned int create_shader(const std::string &vertexShader , const std::string &fragmentShader)//compile the two shaders
{

    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program , vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}

int main()//main function
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "My Game Engine", NULL, NULL);//create a window

    glfwSetWindowShouldClose(window, false);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();//terminate application
        return -1;
    }

    glfwMakeContextCurrent(window);//made a context
   
     //check if glew is ok

    if (glewInit() != GLEW_OK)std::cout << "Glew is not ok" << std::endl;
   
    glViewport(0, 0, 800, 600);//set the viewport

    //resizing screen
    void framebuffer_size_callback(GLFWwindow * window, int width, int height);//declare function

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //positions

    float positions[6] =
    {

        +0.0f, +1.0f,
        -1.0f, -1.0f,
        +1.0f, -1.0f

    };

    //vertex array object

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertex buffer

    unsigned int VBO;
    glGenBuffers(1 , &VBO);
    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) , positions , GL_STATIC_DRAW);


    //vertex attrib array

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , 2*sizeof(float) , 0);


    //some gpu drivers provide you with a default shader

    //shaders
    std::string vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f , 0.0f , 0.0f , 0.0f);\n"
        "}\n";

    unsigned int shader = create_shader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))//render loop
    {

        processInput(window);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //drawing with legacy opengl
        /**
        glBegin(GL_TRIANGLES);

        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);

        glEnd();
        */

        glfwSwapBuffers(window);//swaps buffers to overcome flickering issues
        glfwPollEvents();//waits for events

    }

    glDeleteProgram(shader);
    glDeleteBuffers(1 , &VBO);
    glDeleteBuffers(1, &VAO);

    glfwTerminate();//terminate glfw

    return 0;//return code zero to exit safely

}

