#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "charactermap.h"
#include "helpers.h"
#include "button.h"
#include "settings.h"
#include "renderer.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int b, int action, int mods);
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);




Renderer renderer;

Button button;


int main(){


    glfwInit();

    //version 3.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //apple only
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "C UI", NULL, NULL);

    if(window == NULL){
        printf("Error creating window");
        glfwTerminate();
        return 1;
    }

    

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize glad");
        glfwTerminate();
        return 1;
    }

    //set view port
    
    glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    renderer = Create_Renderer();

    button = Create_Button(&renderer, "Click Me!", "assets/fonts/Inter.ttf", 10.0f, 100.0f, 120.0f, 40.0f, (vec4){0.0f, 0.0f, 1.0f, 1.0f}, (vec3){1.0f, 1.0f, 1.0f});

    
    Button_SetCenterText(&button, true);
       

    // float vertices[] = {
    //     //pos                   colors              texture coords
    //      0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 0.7f,        //top right
    //     -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 0.7f,        //top left
    //     -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.7f,        //bottom left
    //      0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.7f,        //bottom right
    // };

    Renderer_PushButton(&renderer, &button);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double mouse_x, mouse_y;
    while(!glfwWindowShouldClose(window))
    {
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        processInput(window);
        Renderer_ProcessInput(&renderer, &button, mouse_x, mouse_y);

        
        
        //printf("Mouse X: %.2lf Mouse y: %.2lf\n", convertPixelXToNDC(mouse_x), convertPixelYToNDC(mouse_y));
        //printf("Button X: %.2f Button Y: %.2f\n", convertPixelXToNDC(button.x), convertPixelYToNDC(button.y));

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        Renderer_DrawButton(&renderer, &button);

        glfwSwapBuffers(window);
        glfwPollEvents();



    }

    glfwTerminate();

    free(button.vertices);

    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void processInput(GLFWwindow* window)
{

}


static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void mouse_button_callback(GLFWwindow* window, int b, int action, int mods)
{
    if (b == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){

        //sprintf("%d\n", button.isHovered);
        if(button.isHovered){
            printf("You clicked Me!\n");
        }
    }
}