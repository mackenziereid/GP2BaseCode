#ifndef Texture_h
#define Texture_h

#include "Common.h"

GLuint loadTextureFromFile(const string& filename);

GLuint convertSDLSurfaceToTexture(SDL_Surface * surface);

#endif
