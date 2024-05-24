
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\0";

//We can also call a function that updates the viewport if the windows is resized using:
void framebuffer_size_callback(GLFWwindow* window, int widht, int height);

//To handle user input:
void processInput(GLFWwindow* window);

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600, "Learn Open Gl", NULL,NULL);
    if(window == NULL){
        std::cout <<"Failed to create a Window" << std::endl;
        glfwTerminate();
        return -1; //This mean an error

    }
    glfwMakeContextCurrent(window);//This sets our newly created window as the maincontext of the thread

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout <<"Failed to initialize GLAD" <<std::endl;
        return -1;
    }

    //We need to tell OpenGl how we want to display the data within the window using:
    glViewport(0,0,800,600);
    //We can also call a function that updates the viewport if the windows is resized using:
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);


    
//VERTEX SHADER
//-------------------------------------------------------------------------------------------------

float vertices[] = {
0.5f, 0.5f, 0.0f, // top right
0.5f, -0.5f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, // bottom left
-0.5f, 0.5f, 0.0f // top left
}; //This are then transformed into screen-space coordinates via teh viewport transform (using the glViewport data)
   
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int VBO;//We declare our vertex buffer object

    glGenBuffers(1,&VBO);//We generate a buffer with a unique ID

    glBindBuffer(GL_ARRAY_BUFFER,VBO);//There is different buffer objects, and a vertex buffer object is GL_ARRAY_BUFFER
    //With this function we can bind the newly created buffer to this buffer object so any call we make will be used to configure

    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//This function copies user-defined data into the bound buffer

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//We pass the type of shader as an argument

    //This function takes the shader object that is gonna be compiled, then the number of strings as source code
    //Then the source code itself/
    glShaderSource(vertexShader,1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //To check if the compilation was succesful we can do something like this:
    int success;
    char infoLogs[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLogs);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION:FAILED\n" << infoLogs << std::endl;

    }

    //--------------------------------------------------------------------------------------

    //FRAGMENT SHADER this is used to represent colors
    //--------------------------------------------------------------------------------------
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    //--------------------------------------------------------------------------------------

    //SHADER PROGRAM this is used to created the final linked version of multiple shaders combined
    //--------------------------------------------------------------------------------------

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//This return an ID for the program
    //We need to attach out shaders into the program:
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    


    //We are almost there, we now need to tell OpenGl how to interpret the vertex data before rendering

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLogs);
        std::cout<<"ERROR::SHADER::PROGRAM::COMPILATION:FAILED\n" << infoLogs << std::endl;
    }
    glUseProgram(shaderProgram);
    
    //We can delete the shader objects once weve linked them into the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //--------------------------------------------------------------------------------------

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    
    unsigned int EBO;
    glGenBuffers(1,&EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
 
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
//
   glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    
    //We can now create the RENDER LOOP    
    while(!glfwWindowShouldClose(window))//An iteration of this loop is what is considered a frame :)
    {
        //input
        processInput(window);


        //rendering...
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        //check and call events and swap the buffers
        glfwSwapBuffers(window);//Thos swaps colo buffers
        glfwPollEvents();//This function checks if there is any event triggered
        
    }
    //To terminate and free all glfw resources we can use:
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int widht, int height)
{
    glViewport(0,0,widht,height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) //Thos checks if we have pressed the escape key, if not it return
    //GLFW_RELEASE, but if we did, we close GLFW using glfwSetWindowShouldClose
    {
        glfwSetWindowShouldClose(window,true);
    }
}
