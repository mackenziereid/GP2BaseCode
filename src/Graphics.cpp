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
    //screen ration
    GLfloat ratio;
    
    //make sure height is always above 1
    if (height == 0) {
        height = 1;
    }
    
    //calculate screen ration
    ratio = (GLfloat) width/ (GLfloat) height;
    
    //setup viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    //change to projection matrix mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //calculate perspective matrix using glu library functions
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    
    //switch to model view
    glMatrixMode(GL_MODELVIEW);
    
    //Reset using the identity matrix
    glLoadIdentity();
}