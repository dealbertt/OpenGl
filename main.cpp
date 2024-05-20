#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
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


    //We can now create the RENDER LOOP
    while(!glfwWindowShouldClose(window))//An iteration of this loop is what is considered a frame :)
    {
        //input
        processInput(window);


        //rendering...
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        //check and call events and swap the buffers
        glfwSwapBuffers(window);//Thos swaps colo buffers
        glfwPollEvents();//This function checks if there is any event triggered
        
    }
    //To terminate and free all glfw resources we can use:
    glfwTerminate();

    float vertices[] = {//Because we render in 2d, each vertex has a z coordinate of 0.0
    //This coordinate are known as Normalized Device Coordinates which is a small place where the x, y and z
    //values vary from -1.0 to 1.0. Everything outside this range is discarded
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    }; //This are then transformed into screen-space coordinates via teh viewport transform (using the glViewport data)
   

    unsigned int VBO;//We declare our vertex buffer object

    glGenBuffers(1,&VBO);//We generate a buffer with a unique ID

    glBindBuffer(GL_ARRAY_BUFFER,VBO);//There is different buffer objects, and a vertex buffer object is GL_ARRAY_BUFFER
    //With this function we can bind the newly created buffer to this buffer object so any call we make will be used to configure

    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//This function copies user-defined data into the bound buffer


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
