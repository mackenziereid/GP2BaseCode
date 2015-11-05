#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#elif (_WIN64 || _WIN32)
#include <gl/GLU.h>
#endif

#endif
