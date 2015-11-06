#include "Graphics.h"
#include "Common.h"

//Function to initilize OpenGL
void initOpenGL()
{
    //Smooth Shading
    glShadeModel(GL_SMOOTH);
    
    //clear the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    //clear the depth buffer to 1.0
    glClearDepth(1.0f);
    
    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    //the depth test to use
    glDepthFunc(GL_LEQUAL);
    
    //Turn on the best perspetive mode
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        //problem: glewinit failed, something is seriously wrong
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    
}

void setViewport(int width, int height)
{
    //setup viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}